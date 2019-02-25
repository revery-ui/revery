module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Revery_Math;

module UniqueId =
  Revery_Core.UniqueId.Make({});

type callback = unit => unit;

type cachedNodeState = {
  transform: Mat4.t,
  worldTransform: Mat4.t,
  bbox: BoundingBox2d.t,
  depth: int,
};

exception NoDataException(string);
let getOrThrow: (string, option('a)) => 'a =
  (msg, opt) =>
    switch (opt) {
    | Some(p) => p
    | None => raise(NoDataException(msg))
    };

class node ('a) (()) = {
  as _this;
  val _children: ref(list(node('a))) = ref([]);
  val _style: ref(Style.t) = ref(Style.defaultStyle);
  val _events: ref(NodeEvents.t(node('a))) = ref(NodeEvents.make());
  val _layoutNode = ref(Layout.createNode([||], Layout.defaultStyle));
  val _parent: ref(option(node('a))) = ref(None);
  val _internalId: int = UniqueId.getUniqueId();
  val _tabIndex: ref(option(int)) = ref(None);
  val _hasFocus: ref(bool) = ref(false);
  val _cachedNodeState: ref(option(cachedNodeState)) = ref(None);
  val _queuedCallbacks: ref(list(callback)) = ref([]);
  val _lastDimensions: ref(NodeEvents.DimensionsChangedEventParams.t) =
    ref(NodeEvents.DimensionsChangedEventParams.create());
  val _miniLayout: ref(Dimensions.t) = ref(Dimensions.create(~top=0, ~left=0, ~width=0, ~height=0, ()));
  pub draw = (pass: 'a, parentContext: NodeDrawContext.t) => {
    let style: Style.t = _this#getStyle();
    let worldTransform = _this#getWorldTransform();
    let dimensions = _this#measurements();

    Overflow.render(
      worldTransform,
      style.overflow,
      dimensions,
      parentContext.screenHeight,
      parentContext.pixelRatio,
      parentContext.scaleFactor,
      () => {
        let localContext =
          NodeDrawContext.createFromParent(parentContext, style.opacity);
        List.iter(c => c#draw(pass, localContext), _children^);
      },
    );
  };
  pub measurements = () => {
      let style = _this#getStyle();
    let ret: Dimensions.t =  switch (style.layoutMode) {
    | Style.LayoutMode.Minimal => _miniLayout^;
    | Style.LayoutMode.Default => {
        let layout = _layoutNode^.layout;
        Dimensions.create(~left=layout.left, ~top=layout.top, ~width=layout.width, ~height=layout.height, ());
    }
    };
    ret;
  };
  pub getInternalId = () => _internalId;
  pub getTabIndex = () => _tabIndex^;
  pub setTabIndex = index => _tabIndex := index;
  pub setStyle = style => _style := style;
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
  pri _recalculateDepth = () =>
    switch (_parent^) {
    | None => 0
    | Some(p) => p#getDepth() + 1
    };
  pub recalculate = () => {
    let transform = _this#_recalculateTransform();
    let worldTransform = _this#_recalculateWorldTransform(transform);
    let bbox = _this#_recalculateBoundingBox(worldTransform);
    let depth = _this#_recalculateDepth();

    _cachedNodeState := Some({transform, worldTransform, bbox, depth});

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
    let bbox = _this#getBoundingBox();
    BoundingBox2d.isPointInside(bbox, p);
  };
  pub addChild = (n: node('a)) => {
    _children := List.append(_children^, [n]);
    n#_setParent(Some((_this :> node('a))));
  };
  pub removeChild = (n: node('a)) => {
    _children :=
      List.filter(c => c#getInternalId() != n#getInternalId(), _children^);
    n#_setParent(None);
  };
  pub firstChild = () => List.hd(_children^);
  pub getParent = () => _parent^;
  pub getChildren = () => _children^;
  pub getMeasureFunction = (_pixelRatio: float, _scaleFactor: int) => None;
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
  pub _minimalLayout =  (style: Style.t) => {
    let prev = _miniLayout^;
    if (prev.top != style.top || prev.left != style.left || prev.width != style.width || prev.height != style.height) {
        _miniLayout := Dimensions.create(~top=style.top, ~left=style.left, ~width=style.width, ~height=style.height, ());
    };
    List.iter((n) => n#_minimalLayout(style), _children^);
  };
  pub toLayoutNode = (pixelRatio: float, scaleFactor: int) => {
    let style = _style^;

    let f = (v) => switch(v) {
    | Some(_) => true
    | None => false
    };

    let m = (v) => switch(v) {
    | Some(v) => v
    | None => Layout.createNode([||], Style.toLayoutNode(style));
    }

    switch (style.layoutMode) {
    | Style.LayoutMode.Minimal => {
        _this#_minimalLayout(style); 
        None;
    }
    | Style.LayoutMode.Default => {
        let childNodes =
          List.map(c => c#toLayoutNode(pixelRatio, scaleFactor), _children^)
          |> List.filter(f)
          |> List.map(m);
          
        let layoutStyle = Style.toLayoutNode(_style^);
        let node =
          switch (_this#getMeasureFunction(pixelRatio, scaleFactor)) {
          | None => Layout.createNode(Array.of_list(childNodes), layoutStyle)
          | Some(m) =>
            Layout.createNodeWithMeasure(
              Array.of_list(childNodes),
              layoutStyle,
              m,
            )
          };

        _layoutNode := node;
        Some(node);
    };
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
  pub _setParent = (n: option(node('a))) => {
    _parent := n;

    /* Dispatch ref event if we just got attached */
    switch (n) {
    | Some(_) =>
      let ret = (_this :> node('a));
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

let iter = (f, node: node('a)) => {
  let rec apply = node => {
    f(node);

    let children = node#getChildren();
    List.iter(apply, children);
  };

  apply(node);
};
