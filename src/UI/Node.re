module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;
module RenderPass = Revery_Draw.RenderPass;

open Revery_Math;

module UniqueId =
  Revery_Core.UniqueId.Make({});

type callback = unit => unit;

type cachedNodeState = {
  transform: Mat4.t,
  worldTransform: Mat4.t,
  bbox: BoundingBox2d.t,
  bboxClipped: BoundingBox2d.t,
  depth: int,
};

exception NoDataException(string);
let getOrThrow: (string, option('a)) => 'a =
  (msg, opt) =>
    switch (opt) {
    | Some(p) => p
    | None => raise(NoDataException(msg))
    };

class node (()) = {
  as _this;
  val _children: ref(list(node)) = ref([]);
  val _style: ref(Style.t) = ref(Style.defaultStyle);
  val _layoutStyle: ref(LayoutTypes.cssStyle) =
    ref(Layout.LayoutSupport.defaultStyle);
  val _events: ref(NodeEvents.t(node)) = ref(NodeEvents.make());
  val _layoutNode = ref(Layout.createNode([||], Layout.defaultStyle));
  val _parent: ref(option(node)) = ref(None);
  val _internalId: int = UniqueId.getUniqueId();
  val _tabIndex: ref(option(int)) = ref(None);
  val _hasFocus: ref(bool) = ref(false);
  val _cachedNodeState: ref(option(cachedNodeState)) = ref(None);
  val _queuedCallbacks: ref(list(callback)) = ref([]);
  val _lastDimensions: ref(NodeEvents.DimensionsChangedEventParams.t) =
    ref(NodeEvents.DimensionsChangedEventParams.create());
  val _isLayoutDirty = ref(true);
  val _forcedMeasurements: ref(option(Dimensions.t)) = ref(None);
  pub draw = (parentContext: NodeDrawContext.t) => {
    let style: Style.t = _this#getStyle();
    let worldTransform = _this#getWorldTransform();
    let dimensions = _this#measurements();

    let ctx = RenderPass.getContext();

    Overflow.render(
      worldTransform,
      style.overflow,
      dimensions,
      ctx.screenHeight,
      ctx.pixelRatio,
      ctx.scaleFactor,
      () => {
        let localContext =
          NodeDrawContext.createFromParent(parentContext, style.opacity);
        List.iter(c => c#draw(localContext), _children^);
      },
    );
  };
  pub measurements = () => {
    switch (_forcedMeasurements^) {
    | Some(v) => v
    | None =>
      let layout = _layoutNode^.layout;
      Dimensions.create(
        ~left=layout.left,
        ~top=layout.top,
        ~width=layout.width,
        ~height=layout.height,
        (),
      );
    };
  };
  pub forceMeasurements = (dimensions: Dimensions.t) => {
    _forcedMeasurements := Some(dimensions);
  };
  pub getInternalId = () => _internalId;
  pub getTabIndex = () => _tabIndex^;
  pub setTabIndex = index => _tabIndex := index;
  pub markLayoutDirty = () => {
    _isLayoutDirty^
      ? ()
      : {
        switch (_this#getParent()) {
        | Some(p) => p#markLayoutDirty()
        | None => ()
        };
        _isLayoutDirty := true;
      };
  };
  pub setStyle = style =>
    if (style != _style^) {
      _style := style;

      let lastLayoutStyle = _layoutStyle^;
      let newLayoutStyle = Style.toLayoutNode(style);
      _layoutStyle := newLayoutStyle;

      if (lastLayoutStyle != _layoutStyle^) {
        _this#markLayoutDirty();
      };
    };
  pub getStyle = () => _style^;
  pub setEvents = events => _events := events;
  pub getEvents = () => _events^;
  pub getWorldTransform = () => {
    let state = _cachedNodeState^ |> getOrThrow("getWorldTransform");
    state.worldTransform;
  };
  pub getTransform = () => {
    let state = _cachedNodeState^ |> getOrThrow("getTransform");
    state.transform;
  };
  pub getBoundingBox = () => {
    let state = _cachedNodeState^ |> getOrThrow("getBoundingBox");
    state.bbox;
  };
  pub getBoundingBoxClipped = () => {
    let state = _cachedNodeState^ |> getOrThrow("getBoundingBoxClipped");
    state.bboxClipped;
  };
  pub getDepth = () => {
    let state = _cachedNodeState^ |> getOrThrow("getDepth");
    state.depth;
  };
  pri _recalculateTransform = () => {
    let dimensions = _this#measurements();
    let matrix = Mat4.create();
    Mat4.fromTranslation(
      matrix,
      Vec3.create(
        float_of_int(dimensions.left),
        float_of_int(dimensions.top),
        0.,
      ),
    );
    let animationTransform =
      Transform.toMat4(
        float_of_int(dimensions.width) /. 2.,
        float_of_int(dimensions.height) /. 2.,
        _this#getStyle().transform,
      );
    Mat4.multiply(matrix, matrix, animationTransform);
    matrix;
  };
  pri _recalculateWorldTransform = localTransform => {
    let xform = localTransform;
    let world =
      switch (_parent^) {
      | None => Mat4.create()
      | Some(p) => p#getWorldTransform()
      };
    let matrix = Mat4.create();
    Mat4.multiply(matrix, world, xform);
    matrix;
  };
  pri _recalculateBoundingBox = worldTransform => {
    let dimensions = _this#measurements();
    let min = Vec2.create(0., 0.);
    let max =
      Vec2.create(
        float_of_int(dimensions.width),
        float_of_int(dimensions.height),
      );
    let b = BoundingBox2d.create(min, max);
    let bbox = BoundingBox2d.transform(b, worldTransform);
    bbox;
  };
  pri _recalculateBoundingBoxClipped = worldTransform => {
    let dimensions = _this#measurements();
    let min = Vec2.create(0., 0.);
    let max =
      Vec2.create(
        float_of_int(dimensions.width),
        float_of_int(dimensions.height),
      );
    let b = BoundingBox2d.create(min, max);
    let bbox = BoundingBox2d.transform(b, worldTransform);
    switch (_this#getParent()) {
    | Some(p) => BoundingBox2d.intersect(bbox, p#getBoundingBoxClipped())
    | None => bbox
    };
  };
  pri _recalculateDepth = () =>
    switch (_parent^) {
    | None => 0
    | Some(p) => p#getDepth() + 1
    };
  pub recalculate = () => {
    _isLayoutDirty := false;
    let transform = _this#_recalculateTransform();
    let worldTransform = _this#_recalculateWorldTransform(transform);
    let bbox = _this#_recalculateBoundingBox(worldTransform);
    let bboxClipped = _this#_recalculateBoundingBoxClipped(worldTransform);
    let depth = _this#_recalculateDepth();

    _cachedNodeState :=
      Some({transform, worldTransform, bbox, bboxClipped, depth});

    List.iter(c => c#recalculate(), _children^);

    /* Check if dimensions are different, if so, we need to queue up a dimensions changed event */
    let lastDimensions = _lastDimensions^;
    let newDimensions = _this#measurements();

    if (lastDimensions.width != newDimensions.width
        || lastDimensions.height != newDimensions.height) {
      let maybeOnDimensionsChanged = _this#getEvents().onDimensionsChanged;
      let evt: NodeEvents.DimensionsChangedEventParams.t = {
        width: newDimensions.width,
        height: newDimensions.height,
      };
      _lastDimensions := evt;
      switch (maybeOnDimensionsChanged) {
      | Some(cb) =>
        /*
         * Defer dispatching the `ref` until AFTER layout has occurred.
         * A common use-case for using the ref will be getting dimension
         * and layout information. This won't be available until AFTER
         * layout.
         */
        _this#_queueCallback(() => cb(evt))
      | None => ()
      };
    };
  };
  pub getCursorStyle = () => {
    switch (_this#getStyle().cursor, _this#getParent()) {
    | (None, None) => Revery_Core.MouseCursors.arrow
    | (None, Some(parent)) => parent#getCursorStyle()
    | (Some(cursorStyle), _) => cursorStyle
    };
  };
  pub hasRendered = () => {
    switch (_cachedNodeState^) {
    | Some(_) => true
    | None => false
    };
  };
  pub hitTest = (p: Vec2.t) => {
    let bboxClipped = _this#getBoundingBoxClipped();
    BoundingBox2d.isPointInside(bboxClipped, p);
  };
  pub addChild = (n: node) => {
    _children := List.append(_children^, [n]);
    n#_setParent(Some((_this :> node)));
    _this#markLayoutDirty();
  };
  pub removeChild = (n: node) => {
    _children :=
      List.filter(c => c#getInternalId() != n#getInternalId(), _children^);
    n#_setParent(None);
    _this#markLayoutDirty();
  };
  pub firstChild = () => List.hd(_children^);
  pub getParent = () => _parent^;
  pub getChildren = () => _children^;
  pub getMeasureFunction = () => None;
  pub handleEvent = (evt: NodeEvents.event) => {
    let _ =
      switch (evt, _this#getEvents()) {
      | (MouseDown(c), {onMouseDown: Some(cb), _}) => cb(c)
      | (MouseMove(c), {onMouseMove: Some(cb), _}) => cb(c)
      | (MouseUp(c), {onMouseUp: Some(cb), _}) => cb(c)
      | (MouseWheel(c), {onMouseWheel: Some(cb), _}) => cb(c)
      | (MouseEnter(c), {onMouseEnter: Some(cb), _}) => cb(c)
      | (MouseLeave(c), {onMouseLeave: Some(cb), _}) => cb(c)
      | (MouseOver(c), {onMouseOver: Some(cb), _}) => cb(c)
      | (MouseOut(c), {onMouseOut: Some(cb), _}) => cb(c)
      | (MouseDown(_), _)
      | (MouseMove(_), _)
      | (MouseUp(_), _)
      | (MouseEnter(_), _)
      | (MouseLeave(_), _)
      | (MouseOver(_), _)
      | (MouseOut(_), _)
      | (MouseWheel(_), _) => ()
      | (Focus, p) =>
        _this#focus();
        switch (p) {
        | {onFocus: Some(cb), _} => cb()
        | _ => ()
        };
      | (Blur, p) =>
        _this#blur();
        switch (p) {
        | {onBlur: Some(cb), _} => cb()
        | _ => ()
        };
      | (KeyDown(e), {onKeyDown: Some(cb), _}) => cb(e)
      | (KeyUp(e), {onKeyUp: Some(cb), _}) => cb(e)
      | (KeyPress(e), {onKeyPress: Some(cb), _}) => cb(e)
      | (KeyDown(_), _)
      | (KeyUp(_), _)
      | (KeyPress(_), _) => ()
      };
    ();
  };
  pub toLayoutNode = (~force, ()) => {
    let layoutStyle = _layoutStyle^;

    switch (_isLayoutDirty^ || force) {
    | false => _layoutNode^
    | true =>
      let childNodes = List.map(c => c#toLayoutNode(~force, ()), _children^);

      let node =
        switch (_this#getMeasureFunction()) {
        | None => Layout.createNode(Array.of_list(childNodes), layoutStyle)
        | Some(m) =>
          Layout.createNodeWithMeasure(
            Array.of_list(childNodes),
            layoutStyle,
            m,
          )
        };

      _layoutNode := node;
      node;
    };
  };
  pri _queueCallback = (cb: callback) => {
    _queuedCallbacks := List.append([cb], _queuedCallbacks^);
  };
  pub flushCallbacks = () => {
    let f = cb => cb();
    List.iter(f, _queuedCallbacks^);
    _queuedCallbacks := [];

    let fc = c => c#flushCallbacks();
    List.iter(fc, _children^);
  };
  /* TODO: This should really be private - it should never be explicitly set */
  pub _setParent = (n: option(node)) => {
    _parent := n;

    /* Dispatch ref event if we just got attached */
    switch (n) {
    | Some(_) =>
      let ret = (_this :> node);
      let maybeRef = _this#getEvents().ref;

      switch (maybeRef) {
      | Some(ref) =>
        /*
         * Defer dispatching the `ref` until AFTER layout has occurred.
         * A common use-case for using the ref will be getting dimension
         * and layout information. This won't be available until AFTER
         * layout.
         */
        _this#_queueCallback(() => ref(ret))
      | None => ()
      };
    | _ => ()
    };

    _this#markLayoutDirty();
  };
  pub canBeFocused = () =>
    switch (_tabIndex^) {
    | Some(_) => true
    | None => false
    };
  pub focus = () => {
    _hasFocus := true;
  };
  pub blur = () => {
    _hasFocus := false;
  };
};

let iter = (f, node: node) => {
  let rec apply = node => {
    f(node);

    let children = node#getChildren();
    List.iter(apply, children);
  };

  apply(node);
};
