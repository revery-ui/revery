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

class stack ('a) (init) = {
  as _;
  val mutable v: list('a) = init;
  pub pop =
    switch (v) {
    | [hd, ...tl] =>
      v = tl;
      Some(hd);
    | [] => None
    };
  pub push = hd => v = [hd, ...v];
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
  val mutable _children: list(node('a)) = [];
  val mutable _style: Style.t = Style.defaultStyle;
  val mutable _events: NodeEvents.t(node('a)) = NodeEvents.make();
  val mutable _layoutNode = Layout.createNode([||], Layout.defaultStyle, -1);
  val mutable _parent: option(node('a)) = None;
  val mutable _internalId: int = UniqueId.getUniqueId();
  val mutable _tabIndex: option(int) = None;
  val mutable _hasFocus: bool = false;
  val mutable _cachedNodeState: option(cachedNodeState) = None;
  val mutable _queuedCallbacks: list(callback) = [];
  pub draw = (pass: 'a, parentContext: NodeDrawContext.t) => {
    let style: Style.t = _this#getStyle();
    let worldTransform = _this#getWorldTransform();
    let dimensions = _layoutNode.layout;

    Overflow.render(
      worldTransform,
      style.overflow,
      dimensions,
      parentContext.screenHeight,
      parentContext.pixelRatio,
      () => {
        let localContext =
          NodeDrawContext.createFromParent(parentContext, style.opacity);
        List.iter(c => c#draw(pass, localContext), _children);
      },
    );
  };
  pub getInternalId = () => _internalId;
  pub getLayoutNode = () => _layoutNode;
  pub measurements = () => _layoutNode.layout;
  pub getTabIndex = () => _tabIndex;
  pub setTabIndex = index => _tabIndex = index;
  pub setStyle = style => {
    _style = style;
    _layoutNode.style = Style.toLayoutNode(style);
    Layout.LayoutSupport.markDirtyInternal(_layoutNode);
  };
  pub getStyle = () => _style;
  pub setEvents = events => _events = events;
  pub getEvents = () => _events;
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
  pub getDepth = () => {
    let state = _cachedNodeState |> getOrThrow("getDepth");
    state.depth;
  };
  pri _recalculateTransform = () => {
    let dimensions = _layoutNode.layout;
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
      switch (_parent) {
      | None => Mat4.create()
      | Some(p) => p#getWorldTransform()
      };
    let matrix = Mat4.create();
    Mat4.multiply(matrix, world, xform);
    matrix;
  };
  pri _recalculateBoundingBox = worldTransform => {
    let dimensions = _layoutNode.layout;
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
    switch (_parent) {
    | None => 0
    | Some(p) => p#getDepth() + 1
    };
  pub recalculate = () => {
    let transform = _this#_recalculateTransform();
    let worldTransform = _this#_recalculateWorldTransform(transform);
    let bbox = _this#_recalculateBoundingBox(worldTransform);
    let depth = _this#_recalculateDepth();

    _cachedNodeState = Some({transform, worldTransform, bbox, depth});

    List.iter(c => c#recalculate(), _children);
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
  pub hitTest = (p: Vec2.t) => {
    let bbox = _this#getBoundingBox();
    BoundingBox2d.isPointInside(bbox, p);
  };
  pub addChild = (n: node('a)) => {
    _children = List.append(_children, [n]);
    n#_setParent(Some((_this :> node('a))));
  };
  pub removeChild = (n: node('a)) => {
    _children =
      List.filter(c => c#getInternalId() != n#getInternalId(), _children);
    n#_setParent(None);
  };
  pub firstChild = () => List.hd(_children);
  pub getParent = () => _parent;
  pub getChildren = () => _children;
  pub getMeasureFunction = (_pixelRatio: float) => None;
  pub handleEvent = (evt: NodeEvents.event) => {
    let _ =
      switch (evt, _this#getEvents()) {
      | (MouseDown(c), {onMouseDown: Some(cb), _}) => cb(c)
      | (MouseMove(c), {onMouseMove: Some(cb), _}) => cb(c)
      | (MouseUp(c), {onMouseUp: Some(cb), _}) => cb(c)
      | (MouseWheel(c), {onMouseWheel: Some(cb), _}) => cb(c)
      | (MouseDown(_), _)
      | (MouseMove(_), _)
      | (MouseUp(_), _)
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
      };
    ();
  };
  pub createLayoutNode = pixelRatio => {
    let layoutStyle = Style.toLayoutNode(_style);
    let node =
      switch (_this#getMeasureFunction(pixelRatio)) {
      | None => Layout.createNode([||], layoutStyle, _this#getInternalId())
      | Some(m) =>
        Layout.createNodeWithMeasure(
          [||],
          layoutStyle,
          m,
          _this#getInternalId(),
        )
      };

    _layoutNode = node;
    node;
  };
  pub addChildLayoutNode = (child: node('a)) => {
    _layoutNode.children =
      Array.append(_layoutNode.children, [|child#getLayoutNode()|]);
    _layoutNode.childrenCount = _layoutNode.childrenCount + 1;
    child#getLayoutNode().parent = _layoutNode;
    Layout.LayoutSupport.markDirtyInternal(_layoutNode);
  };
  pub removeChildLayoutNode = (child: node('a)) => {
    _layoutNode.childrenCount = _layoutNode.childrenCount - 1;
    _layoutNode.children =
      _layoutNode.children
      |> Array.to_list
      |> List.filter((n: Layout.LayoutTypes.node) =>
           n.context != child#getInternalId()
         )
      |> Array.of_list;
    Layout.LayoutSupport.markDirtyInternal(_layoutNode);
  };
  pri _queueCallback = (cb: callback) => {
    _queuedCallbacks = List.append([cb], _queuedCallbacks);
  };
  pub flushCallbacks = () => {
    let f = cb => cb();
    List.iter(f, _queuedCallbacks);
    _queuedCallbacks = [];

    let fc = c => c#flushCallbacks();
    List.iter(fc, _children);
  };
  /* TODO: This should really be private - it should never be explicitly set */
  pub _setParent = (n: option(node('a))) => {
    _parent = n;

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
};

let iter = (f, node: node('a)) => {
  let rec apply = node => {
    f(node);

    let children = node#getChildren();
    List.iter(apply, children);
  };

  apply(node);
};