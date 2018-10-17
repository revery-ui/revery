let render = () => {
    print_endline("Hello, world!");
};

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;

class node (name: string) = {
    as _this;
    
    val _children: ref(list(node)) = ref([]);

    pub draw = (layer: int) => {
        print_endline("- Drawing: " ++ name ++ " | " ++ string_of_int(layer));
        List.iter((c) => c#draw(layer + 1), _children^)
    };

    pub addChild = (n: node) => {
        _children := List.append(_children^, [n]);
    };
};

class viewNode () = {
    as _this;

    val _basicShader = Shaders.BasicShader.create();
    val _quad = Geometry.Cube.create();

    inherit (class node)("viewNode") as _super;
            
    pub! draw = (layer: int) => {
        Shaders.Shader.CompiledShader.use(_basicShader);
        Geometry.draw(_quad, _basicShader);

        _super#draw(layer);
    };
};
