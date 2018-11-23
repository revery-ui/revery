module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Reglm;

class node ('a) (_name: string) = {
    as _this;

    val _children: ref(list(node('a))) = ref([]);
    val _style = ref(Style.defaultStyle);
    val _layoutNode = ref(Layout.createNode([||], Layout.defaultStyle));

    pub draw = (pass: 'a, layer: int, m: Mat4.t) => {
        let dimensions = (_layoutNode^).layout;
        let matrix = Mat4.create();
        Mat4.fromTranslation(matrix, Vec3.create(float_of_int(dimensions.left), float_of_int(dimensions.top), 0.));
        List.iter((c) => {
                let s: Style.t = c#getStyle();
                let xform = Transform.toMat4(s.transform);
                Mat4.multiply(xform, xform, matrix);
                Mat4.multiply(xform, xform, m);
                /* Mat4.multiply(xform, matrix, xform); */
                /* Mat4.multiply(xform, m, xform); */
                c#draw(pass, layer + 1, xform);
        }, _children^)
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
