module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Revery_Math;

module Log = (val Revery_Core.Log.withNamespace("Revery.UI.Node"));

module ListEx = {
  let insert = (i, node, list) => {
    let rec loop = (i, before, after) =>
      if (i > 0) {
        switch (after) {
        | [] => loop(0, before, after)
        | [head, ...tail] => loop(i - 1, [head, ...before], tail)
        };
      } else if (i == 0) {
        loop(i - 1, before, [node, ...after]);
      } else {
        switch (before) {
        | [] => after
        | [head, ...tail] => loop(i, tail, [head, ...after])
        };
      };

    loop(i, [], list);
  };
};

module UniqueId =
  Revery_Core.UniqueId.Make({});

type callback = unit => unit;

exception NoDataException(string);
let getOrThrow: (string, option('a)) => 'a =
  (msg, opt) =>
    switch (opt) {
    | Some(p) => p
    | None => raise(NoDataException(msg))
    };

type cachedNodeState = {
  transform: Skia.Matrix.t,
  worldTransform: Skia.Matrix.t,
  bbox: BoundingBox2d.t,
  bboxClipped: BoundingBox2d.t,
  depth: int,
};
class node (()) = {
  as _this;
  val mutable _children: list(node) = [];
  val mutable _style: Style.t = Style.defaultStyle;
  val mutable _layoutStyle: LayoutTypes.cssStyle = Layout.LayoutSupport.defaultStyle;
  val mutable _events: NodeEvents.t(node) = NodeEvents.make();
  val mutable _layoutNode = Layout.createNode([||], Layout.defaultStyle);
  val mutable _parent: option(node) = None;
  val _internalId = UniqueId.getUniqueId();
  val mutable _tabIndex: option(int) = None;
  val mutable _hasFocus = false;
  val mutable _cachedNodeState: option(cachedNodeState) = None;
  val mutable _queuedCallbacks: list(callback) = [];
  val mutable _lastDimensions: NodeEvents.DimensionsChangedEventParams.t =
    NodeEvents.DimensionsChangedEventParams.create();
  val mutable _isLayoutDirty = true;
  val mutable _forcedMeasurements: option(Dimensions.t) = None;
  val mutable _hasHadNonZeroBlurRadius = false;
  val mutable _mouseBehavior = Sdl2.Window.Normal;
  // !! WARNING !!
  // These values are not marked as [mutable], but they are mutated
  // via the C FFI for performance.
  // They are created once to be re-used across recalculations,
  // and will be mutated when `[recalculate]` is called.
  val _worldTransform = Skia.Matrix.make();
  val _localTransform = Skia.Matrix.make();
  val _bboxLocal = BoundingBox2d.create(0., 0., 0., 0.);
  val _bboxWorld = BoundingBox2d.create(0., 0., 0., 0.);
  val _bboxClipped = BoundingBox2d.create(0., 0., 0., 0.);
  val _lastBoundingBox: BoundingBox2d.t = BoundingBox2d.create(0., 0., 0., 0.);
  pub draw = (parentContext: NodeDrawContext.t) => {
    let style: Style.t = _this#getStyle();
    let worldTransform = _this#getWorldTransform();
    let dimensions = _this#measurements();

    let {canvas, _}: NodeDrawContext.t = parentContext;

    Revery_Draw.CanvasContext.setMatrix(canvas, worldTransform);

    Overflow.render(
      canvas,
      style.overflow,
      dimensions,
      () => {
        let localContext =
          NodeDrawContext.createFromParent(parentContext, style.opacity);
        List.iter(c => c#draw(localContext), _this#getChildren());
      },
    );
  };
  pub measurements = () => {
    switch (_forcedMeasurements) {
    | Some(v) => v
    | None =>
      let layout = _layoutNode.layout;
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
    _forcedMeasurements = Some(dimensions);
  };
  pub getSceneOffsets = () => {
    let Dimensions.{left, top, _} = _this#measurements();
    switch (_parent) {
    | Some(parent) =>
      let parentOffsets = parent#getSceneOffsets();
      Offset.{left: left + parentOffsets.left, top: top + parentOffsets.top};
    | None => Offset.{left, top}
    };
  };
  pub getInternalId = () => _internalId;
  pub getTabIndex = () => _tabIndex;
  pub setTabIndex = index => _tabIndex = index;
  pub markLayoutDirty = () => {
    _isLayoutDirty
      ? ()
      : {
        switch (_this#getParent()) {
        | Some(p) => p#markLayoutDirty()
        | None => ()
        };
        _isLayoutDirty = true;
      };
  };
  pub setStyle = style =>
    if (style != _style) {
      if (style.boxShadow.blurRadius != 0. || _hasHadNonZeroBlurRadius) {
        _hasHadNonZeroBlurRadius = true;
      };
      _style = style;

      let lastLayoutStyle = _layoutStyle;
      let newLayoutStyle = Style.toLayoutNode(style);
      _layoutStyle = newLayoutStyle;

      if (lastLayoutStyle != _layoutStyle) {
        _this#markLayoutDirty();
      };
    };
  pub getStyle = () => {
    _style;
  };
  pub setEvents = events => _events = events;
  pub getEvents = () => _events;
  pub getChildren = () => _children;
  pub getWorldTransform = () => {
    let state = _cachedNodeState |> getOrThrow("getWorldTransform");
    state.worldTransform;
  };
  pub getTransform = () => {
    let state = _cachedNodeState |> getOrThrow("getTransform");
    state.transform;
  };
  pub getBoundingBox = () => {
    let state = _cachedNodeState |> getOrThrow("getBoundingBox");
    state.bbox;
  };
  pub getBoundingBoxClipped = () => {
    let state = _cachedNodeState |> getOrThrow("getBoundingBoxClipped");
    state.bboxClipped;
  };
  pub getDepth = () => {
    let state = _cachedNodeState |> getOrThrow("getDepth");
    state.depth;
  };
  pri _recalculateTransform = () => {
    let dimensions = _this#measurements();
    Skia.Matrix.setTranslate(
      _localTransform,
      dimensions.left |> float_of_int,
      dimensions.top |> float_of_int,
    );

    let transforms = _this#getStyle().transform;
    switch (transforms) {
    // Skip a matrix multiplication if there are no transforms
    | [] => ()
    | _ =>
      let animationTransform =
        Transform.toMat4(
          float_of_int(dimensions.width) /. 2.,
          float_of_int(dimensions.height) /. 2.,
          _this#getStyle().transform,
        );
      Skia.Matrix.preConcat(_localTransform, animationTransform);
    };
    _localTransform;
  };
  pri _recalculateWorldTransform = localTransform => {
    let xform = localTransform;
    let world =
      switch (_parent) {
      | None =>
        Skia.Matrix.setIdentity(_worldTransform);
        _worldTransform;
      | Some(p) => p#getWorldTransform()
      };
    Skia.Matrix.concat(_worldTransform, world, xform);
    _worldTransform;
  };
  pri _recalculateBoundingBox = worldTransform => {
    let dimensions = _this#measurements();
    BoundingBox2d.Mutable.set(
      ~out=_bboxLocal,
      0.,
      0.,
      float_of_int(dimensions.width),
      float_of_int(dimensions.height),
    );
    BoundingBox2d.Mutable.transform(
      ~out=_bboxWorld,
      _bboxLocal,
      worldTransform,
    );
    _bboxWorld;
  };
  pri _recalculateBoundingBoxClipped = bbox => {
    switch (_this#getParent()) {
    | Some(p) =>
      BoundingBox2d.Mutable.intersect(
        ~out=_bboxClipped,
        bbox,
        p#getBoundingBoxClipped(),
      );
      _bboxClipped;
    | None => bbox
    };
  };
  pri _recalculateDepth = () =>
    switch (_parent) {
    | None => 0
    | Some(p) => p#getDepth() + 1
    };
  pub recalculate = () => {
    _isLayoutDirty = false;
    let transform = _this#_recalculateTransform();
    let worldTransform = _this#_recalculateWorldTransform(transform);
    let bbox = _this#_recalculateBoundingBox(worldTransform);
    let bboxClipped = _this#_recalculateBoundingBoxClipped(bbox);
    let depth = _this#_recalculateDepth();

    _cachedNodeState =
      Some({transform, worldTransform, bbox, bboxClipped, depth});

    List.iter(c => c#recalculate(), _children);

    /* Check if dimensions are different, if so, we need to queue up a dimensions changed event */
    let newDimensions = _this#measurements();

    let events = _this#getEvents();

    if (_lastDimensions.width != newDimensions.width
        || _lastDimensions.height != newDimensions.height) {
      let evt: NodeEvents.DimensionsChangedEventParams.t = {
        width: newDimensions.width,
        height: newDimensions.height,
      };
      _lastDimensions = evt;

      /*
       * Defer dispatching the `ref` until AFTER layout has occurred.
       * A common use-case for using the ref will be getting dimension
       * and layout information. This won't be available until AFTER
       * layout.
       */
      switch (events.onDimensionsChanged) {
      | None => ()
      | Some(cb) => _this#_queueCallback(() => cb(evt))
      };
    };

    switch (events.onBoundingBoxChanged) {
    | None => ()
    | Some(cb) =>
      if (!BoundingBox2d.equals(_lastBoundingBox, bbox)) {
        let (x0, y0, x1, y1) = BoundingBox2d.getBounds(bbox);
        BoundingBox2d.Mutable.set(~out=_lastBoundingBox, x0, y0, x1, y1);
        _this#_queueCallback(() => cb(bbox));
      }
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
    switch (_cachedNodeState) {
    | Some(_) => true
    | None => false
    };
  };
  pub hitTest = (x: float, y: float) => {
    let bboxClipped = _this#getBoundingBoxClipped();
    BoundingBox2d.isPointInside(~x, ~y, bboxClipped);
  };
  pub addChild = (child: node, position: int) => {
    _children = ListEx.insert(position, child, _children);
    child#_setParent(Some((_this :> node)));
    _this#markLayoutDirty();
  };
  pub cleanup = () => {
    List.iter(c => c#cleanup(), _children);
  };
  pub removeChild = (n: node) => {
    _children =
      List.filter(c => c#getInternalId() != n#getInternalId(), _children);
    n#cleanup();
    n#_setParent(None);
    _this#markLayoutDirty();
  };
  pub firstChild = () => List.hd(_this#getChildren());
  pub getParent = () => _parent;
  pub getMeasureFunction = () => None;
  pub handleEvent = (evt: NodeEvents.event) => {
    Log.tracef(m =>
      m(
        "Received event on node %i: %s",
        _internalId,
        NodeEvents.show_event(evt),
      )
    );
    prerr_endline (Printf.sprintf("Trying to handle event - %d: %s ", _internalId, NodeEvents.show_event(evt)));
    switch (evt, _this#getEvents()) {
    | (MouseDown(c), {onMouseDown: Some(cb), _}) => cb(c)
    | (MouseMove(c), {onMouseMove: Some(cb), _}) => cb(c)
    | (MouseUp(c), {onMouseUp: Some(cb), _}) => cb(c)
    | (MouseWheel(c), {onMouseWheel: Some(cb), _}) => 
    prerr_endline (Printf.sprintf("--- calling cb for mousewheel - %d", _internalId));
    cb(c)
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
    | (TextInput(e), {onTextInput: Some(cb), _}) => cb(e)
    | (TextEdit(e), {onTextEdit: Some(cb), _}) => cb(e)
    | (TextInput(_), _)
    | (TextEdit(_), _)
    | (KeyDown(_), _)
    | (KeyUp(_), _) => ()
    | (FileDropped(e), {onFileDropped: Some(cb), _}) => cb(e)
    | (FileDropped(_), _) => ()
    };
  };
  pub toLayoutNode = (~force, ()) => {
    let layoutStyle = _layoutStyle;

    if (_isLayoutDirty || force) {
      let childNodes =
        List.map(c => c#toLayoutNode(~force, ()), _this#getChildren());

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

      _layoutNode = node;
      node;
    } else {
      _layoutNode;
    };
  };
  pri _queueCallback = (cb: callback) => {
    _queuedCallbacks = List.append([cb], _queuedCallbacks);
  };
  pub flushCallbacks = () => {
    let f = cb => cb();
    List.iter(f, _queuedCallbacks);
    _queuedCallbacks = [];

    let fc = c => c#flushCallbacks();
    List.iter(fc, _this#getChildren());
  };
  /* TODO: This should really be private - it should never be explicitly set */
  pub _setParent = (n: option(node)) => {
    _parent = n;

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
    switch (_tabIndex) {
    | Some(_) => true
    | None => false
    };
  pub focus = () => {
    _hasFocus = true;
  };
  pub blur = () => {
    _hasFocus = false;
  };
  pub setMouseBehavior = behavior => _mouseBehavior = behavior;
  pub getMouseBehavior = () => _mouseBehavior;
};

let iter = (f, node: node) => {
  let rec apply = node => {
    f(node);

    let children = node#getChildren();
    List.iter(apply, children);
  };

  apply(node);
};
