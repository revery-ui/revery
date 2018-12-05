module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Reglm;

class node ('a) (()) = {
  as _this;
  val _children: ref(list(node('a))) = ref([]);
  val _style: ref(Style.t) = ref(Style.defaultStyle);
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
  pub getLocalTransform = () => {
    let dimensions = _this#measurements();
    /* let left = float_of_int(dimensions.left); */
    /* let top = float_of_int(dimensions.top); */
    let width = float_of_int(dimensions.width);
    let height = float_of_int(dimensions.height);

    let scaleTransform = Mat4.create();
    Mat4.fromScaling(scaleTransform, Vec3.create(width, height, 1.0));

    let animationTransform = Transform.toMat4(_this#getStyle().transform);

    let translateTransform = Mat4.create();
    Mat4.fromTranslation(
      translateTransform,
      Vec3.create(width /. 2., height /. 2., 1.0),
    );

    let world = Mat4.create();
    Mat4.multiply(world, animationTransform, scaleTransform);
    Mat4.multiply(world, translateTransform, world);
    world;
  };
  pub hitTest = (_p: Vec2.t) =>
    /* TODO: Implement hit test against transforms */
    false;
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
  pub getMeasureFunction = () => None;
  pub toLayoutNode = () => {
    let childNodes = List.map(c => c#toLayoutNode(), _children^);
    let layoutStyle = Style.toLayoutNode(_style^);
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
  /* TODO: This should really be private - it should never be explicitly set */
  pub _setParent = (n: option(node('a))) => _parent := n;
};
