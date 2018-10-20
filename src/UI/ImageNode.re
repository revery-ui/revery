open Reglm;
open Reglfw.Glfw;

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Node;
open ViewNode;

class imageNode (name: string, imagePath: string) = {
    as _this;

    val _quad = Geometry.Cube.create();
    val textureShader = TextureShader.create();
    val texture = ImageRenderer.getTexture(imagePath);

    inherit (class node)(name) as _super;
            
    pub! draw = (layer: int) => {
        /* Draw background first */
        _super#draw(layer);

        Shaders.CompiledShader.use(textureShader);
        let projection = Mat4.create();

        Mat4.ortho(projection, 0.0, 800.0, 600.0, 0.0, -0.01, -100.0);
        Shaders.CompiledShader.setUniformMatrix4fv(textureShader, "uProjection", projection);

        let dimensions = _super#measurements();

        Shaders.CompiledShader.setUniform3fv(textureShader, "uColor", Vec3.create(1.0, 1.0, 1.0));

        glBindTexture(GL_TEXTURE_2D, texture);

        Shaders.CompiledShader.setUniform4f(textureShader, "uPosition", 
            float_of_int(dimensions.left),
            float_of_int(dimensions.top),
            float_of_int(dimensions.width),
            float_of_int(dimensions.height)
        );

        Geometry.draw(_quad, textureShader);
    };
};
