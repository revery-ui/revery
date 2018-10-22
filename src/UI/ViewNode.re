open Reglm;

open Revery_Core;

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Node;

class viewNode (name: string) = {
    as _this;

    val _quad = Geometry.Cube.create();
    val solidShader = SolidShader.create();


    inherit (class node)(name) as _super;
            
    pub! draw = (layer: int) => {
        Shaders.CompiledShader.use(solidShader);
        let projection = Mat4.create();
        Mat4.ortho(projection, 0.0, 800.0, 600.0, 0.0, -0.01, -100.0);
        Shaders.CompiledShader.setUniformMatrix4fv(solidShader, "uProjection", projection);

        let style = _super#getStyle();

        let dimensions = _super#measurements();

        print_endline ("** NODE: " ++ name ++ " **");
        print_endline ("-width: " ++ string_of_int(dimensions.width));
        print_endline ("-height: " ++ string_of_int(dimensions.height));

        Shaders.CompiledShader.setUniform3fv(solidShader, "uColor", Color.toVec3(style.backgroundColor));
        Shaders.CompiledShader.setUniform4f(solidShader, "uPosition", 
            float_of_int(dimensions.left),
            float_of_int(dimensions.top),
            float_of_int(dimensions.width),
            float_of_int(dimensions.height));
        Geometry.draw(_quad, solidShader);

        _super#draw(layer);
    };
};
