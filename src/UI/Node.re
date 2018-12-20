module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Revery_Math;

class node ('a) (()) = {
  as _this;
  val _children: ref(list(node('a))) = ref([]);
  val _style: ref(Style.t) = ref(Style.defaultStyle);
  val _events: ref(NodeEvents.t) = ref(NodeEvents.default);
  val _layoutNode = ref(Layout.createNode([||], Layout.defaultStyle));
  val _parent: ref(option(node('a))) = ref(None);
  pub draw = (pass: 'a, parentContext: NodeDrawContext.t) => {
    let style: Style.t = _this#getStyle();
    let localContext =
      NodeDrawContext.createFromParent(parentContext, style.opacity);
    List.iter(c => c#draw(pass, localContext), _children^);
  };
  pub measurements = () => _layoutNode^.layout;
  pub setStyle = style => _style := style;
  pub getStyle = () => _style^;
  pub setEvents = events => _events := events;
  pub getEvents = () => _events^;
  pub getTransform = () => {
    let dimensions = _layoutNode^.layout;
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
  pub getWorldTransform = () => {
    let xform = _this#getTransform();
    let world =
      switch (_parent^) {
      | None => Mat4.create()
      | Some(p) => p#getWorldTransform()
      };
    let matrix = Mat4.create();
    Mat4.multiply(matrix, world, xform);
    matrix;
  };
  pub hitTest = (p: Vec2.t) => {
    let dimensions = _layoutNode^.layout;
    let min = Vec2.create(0., 0.);
    let max =
      Vec2.create(
        float_of_int(dimensions.width),
        float_of_int(dimensions.height),
      );
    let b = BoundingBox2d.create(min, max);
    let bbox = BoundingBox2d.transform(b, _this#getWorldTransform());

    BoundingBox2d.isPointInside(bbox, p);
  };
  pub addChild = (n: node('a)) => {
    _children := List.append(_children^, [n]);
    n#_setParent(Some((_this :> node('a))));
  };
  pub removeChild = (n: node('a)) => {
    _children := List.filter(c => c != n, _children^);
    n#_setParent(None);
  };
  pub getParent = () => _parent^;
  pub getChildren = () => _children^;
  pub getMeasureFunction = (_pixelRatio: int) => None;
  pub handleEvent = (evt: NodeEvents.mouseEvent) => {
    let _ = switch (evt) {
    | MouseDown(c) => _this#getEvents().onMouseDown(c);
    | MouseMove(c) => _this#getEvents().onMouseMove(c);
    | MouseUp(c) => _this#getEvents().onMouseUp(c);
    };
  };
  pub toLayoutNode = (pixelRatio: int) => {
    let childNodes = List.map(c => c#toLayoutNode(pixelRatio), _children^);
    let layoutStyle = Style.toLayoutNode(_style^, pixelRatio);
    let node =
      switch (_this#getMeasureFunction(pixelRatio)) {
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
  /* TODO: This should really be private - it should never be explicitly set */
  pub _setParent = (n: option(node('a))) => _parent := n;
};

let iter = (f, node: node('a)) => {
  let rec apply = (node) => {
    f(node);

    let children = node#getChildren();
    List.iter(apply, children)
  };

  apply(node);
};
