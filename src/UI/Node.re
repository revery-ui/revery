module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

class node (_name: string) = {
    as _this;
    
    val _children: ref(list(node)) = ref([]);
    val _style = ref(Styles.defaultStyle);
    val _layoutNode = ref(Layout.createNode([||], Layout.defaultStyle));

    pub draw = (layer: int) => {
        List.iter((c) => c#draw(layer + 1), _children^)
    };

    pub measurements = () => {
        (_layoutNode^).layout;
    };

    pub setStyle = (style) => {
        _style := style;
    };

    pub addChild = (n: node) => {
        _children := List.append(_children^, [n]);
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
