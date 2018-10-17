let render = () => {
    print_endline("Hello, world!");
};

open Reglm;

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;

class node (name: string) = {
    as _this;
    
    val _children: ref(list(node)) = ref([]);
    val _style = ref(Layout.defaultStyle);
    val _layoutNode = ref(Layout.createNode([||], Layout.defaultStyle));

    pub draw = (layer: int) => {
        print_endline("- Drawing: " ++ name ++ " | " ++ string_of_int(layer));
        List.iter((c) => c#draw(layer + 1), _children^)
    };

    pub measurements = () => {
        Layout.printCssNode(_layoutNode^);
        (_layoutNode^).layout;
    };

    pub setStyle = (style) => {
        _style := style;
    };

    pub addChild = (n: node) => {
        _children := List.append(_children^, [n]);
    };

    pub toLayoutNode = () => {
        let childNodes = List.map((c) => c#toLayoutNode(), _children^);
        let node = Layout.createNode(Array.of_list(childNodes), _style^);
        _layoutNode := node;
        node;
    };
};

class viewNode (name: string, color: Vec3.t) = {
    as _this;

    val _quad = Geometry.Cube.create();
    val solidShader = SolidShader.create();


    inherit (class node)(name) as _super;
            
    pub! draw = (layer: int) => {
        Shaders.CompiledShader.use(solidShader);
        let projection = Mat4.create();
        Mat4.ortho(projection, 0.0, 800.0, 600.0, 0.0, -0.01, -100.0);
        Shaders.CompiledShader.setUniformMatrix4fv(solidShader, "uProjection", projection);

        let dimensions = _super#measurements();

        print_endline ("**" ++ name ++ "**");
        print_endline ("- top: " ++ string_of_int(dimensions.top));
        print_endline ("- left: " ++ string_of_int(dimensions.left));
        print_endline ("- width: " ++ string_of_int(dimensions.width));
        print_endline ("- height: " ++ string_of_int(dimensions.height));

        Shaders.CompiledShader.setUniform3fv(solidShader, "uColor", color);
        Shaders.CompiledShader.setUniform4f(solidShader, "uPosition", 
            float_of_int(dimensions.left),
            float_of_int(dimensions.top),
            float_of_int(dimensions.width),
            float_of_int(dimensions.height));
        Geometry.draw(_quad, solidShader);

        _super#draw(layer);
    };
};

let layout = (node) => {
    let rootLayoutNode = node#toLayoutNode();
    Layout.layoutNode(rootLayoutNode);
    /* Layout.printCssNode(rootLayoutNode); */
};
