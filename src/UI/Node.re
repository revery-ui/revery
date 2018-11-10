module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

/* open Reglfw.Glfw; */

open Reglm;

class node ('a) (_name: string) = {
    as _this;
    
    val _children: ref(list(node('a))) = ref([]);
    val _style = ref(Style.defaultStyle);
    val _layoutNode = ref(Layout.createNode([||], Layout.defaultStyle));

    pub draw = (pass: 'a, layer: int, transform: Mat4.t) => {
        let dimensions = (_layoutNode^).layout;

        let m = Mat4.create();
        Mat4.fromTranslation(m, Vec3.create(dimensions.left, dimensions.top, 0.0));
        Mat4.multiply(m, m, transform);

        List.iter((c) => c#draw(pass, layer + 1, m), _children^);
    };

    pub measurements = () => {
        (_layoutNode^).layout;
    };

    pub setStyle = (style) => {
        _style := style;
    };

    pub getStyle = () => _style^;

    pub addChild = (n: node('a)) => {
        _children := List.append(_children^, [n]);
    };

    pub removeChild = (n: node('a)) => {
        _children := List.filter((c) => c != n, _children^);
    };

    pub getMeasureFunction = () => {
        None;
    };

    pub toLayoutNode = () => {
        let childNodes = List.map((c) => c#toLayoutNode(), _children^);
        let layoutStyle = Style.toLayoutNode(_style^);
        let node = switch (_this#getMeasureFunction()) {
        | None => Layout.createNode(Array.of_list(childNodes), layoutStyle);
        | Some(m) => Layout.createNodeWithMeasure(Array.of_list(childNodes), layoutStyle, m);
        };

        _layoutNode := node;
        node;
    };
};
