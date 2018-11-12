open Reglm;
open Reglfw.Glfw;

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Node;
open ViewNode;
open RenderPass;

class imageNode (name: string, imagePath: string) = {
    as _this;

    val _quad = Geometry.Cube.create();
    val textureShader = TextureShader.create();
    val texture = ImageRenderer.getTexture(imagePath);

    inherit (class node(renderPass))(name) as _super;
            
    pub! draw = (pass: renderPass, layer: int, world: Mat4.t) => {
        /* Draw background first */
        _super#draw(pass, layer, world);

        switch (pass) {
        | SolidPass(m) => {
            Shaders.CompiledShader.use(textureShader);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            Shaders.CompiledShader.setUniformMatrix4fv(textureShader, "uWorld", world);
            Shaders.CompiledShader.setUniformMatrix4fv(textureShader, "uProjection", m);

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
            glDisable(GL_BLEND);

        }
        };

    };
};
