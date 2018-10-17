let render = () => {
    print_endline("Hello, world!");
};

open Reglm;

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

    val _quad = Geometry.Cube.create();
    val solidShader = SolidShader.create();


    inherit (class node)("viewNode") as _super;
            
    pub! draw = (layer: int) => {
        Shaders.CompiledShader.use(solidShader);
        let projection = Mat4.create();
        Mat4.ortho(projection, 0.0, 800.0, 600.0, 0.0, -0.01, -100.0);
        Shaders.CompiledShader.setUniformMatrix4fv(solidShader, "uProjection", projection);
        Shaders.CompiledShader.setUniform3fv(solidShader, "uColor", Vec3.create(1.0, 1.0, 0.0));
        Shaders.CompiledShader.setUniform4f(solidShader, "uPosition", 50., 50., 25., 25.);
        Geometry.draw(_quad, solidShader);

        _super#draw(layer);
    };
};
