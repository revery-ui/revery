module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Reglm;

class node ('a) (_name: string) = {
  as _this;
  val _children: ref(list(node('a))) = ref([]);
  val _style: ref(Style.t) = ref(Style.defaultStyle);
  val _layoutNode = ref(Layout.createNode([||], Layout.defaultStyle));
  pub draw = (pass: 'a, parentContext: NodeDrawContext.t) => {
    let dimensions = _layoutNode^.layout;
    let transform = _this#getTransform();
    let style: Style.t = _this#getStyle();
    let localContext = NodeDrawContext.createFromParent(parentContext, transform, style.opacity);
    List.iter(c => c#draw(pass, localContext), _children^);
  };
  pub measurements = () => _layoutNode^.layout;
  pub setStyle = style => _style := style;
  pub getStyle = () => _style^;
  /* Get world transform */
  /* pub getWorldTransform = () => { */
  /*    let matrix = Mat4.create(); */ 
  /*    Mat4.multiply(matrix, ) */
  /* }; */
  /* Get the transform to be applied to children */
  pub getTransform = () => {
    let matrix = _this#getLocalTransform();
    Mat4.fromTranslation(
      matrix,
      Vec3.create(
        float_of_int(dimensions.left),
        float_of_int(dimensions.top),
        0.,
      ),
    );
  };
  pub getLocalTransform = () => {
    let dimensions = _this#measurements();
    let left = float_of_int(dimensions.left);
    let top = float_of_int(dimensions.top);
    let width = float_of_int(dimensions.width);
    let height = float_of_int(dimensions.height);

    let scaleTransform = Mat4.create();
    Mat4.fromScaling(scaleTransform, Vec3.create(width, height, 1.0));

    let animationTransform = Transform.toMat4(_this#getStyle().transform);

    let translateTransform = Mat4.create();
    Mat4.fromTranslation(
      translateTransform,
      Vec3.create(left +. width /. 2., top +. height /. 2., 1.0),
    );

    let world = Mat4.create();
    Mat4.multiply(world, animationTransform, scaleTransform);
    Mat4.multiply(world, translateTransform, world);
    world;
  };
  pub hitTest = (p: Vec2.t) => {
    /* TODO: Implement hit test against transforms */
    false;
  };
  pub addChild = (n: node('a)) => _children := List.append(_children^, [n]);
  pub removeChild = (n: node('a)) =>
    _children := List.filter(c => c != n, _children^);
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
};
