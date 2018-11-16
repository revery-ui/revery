/**
 * TextureShader
 * 
 * Simple texture shader
 */

open Revery_Shaders;
open Revery_Shaders.Shader;

let attribute: list(ShaderAttribute.t) = SolidShader.attribute;

let uniform: list(ShaderUniform.t) = SolidShader.uniform @ [{
    dataType: ShaderDataType.Sampler2D,
    name: "uSampler",
    usage: FragmentShader,
}];

let varying = SolidShader.varying @ [{
    dataType: ShaderDataType.Vector2,
    name: "vTexCoord",
    precision: ShaderPrecision.Low,
}];


let vsShader = SolidShader.vsShader ++ "\n" ++ {|
   vTexCoord = aTexCoord;
|};

let fsShader = {|
    gl_FragColor = texture2D(uSampler, vTexCoord);
|};

let create = lazy () => {
  let shader =
    Shader.create(
      ~attributes=attribute,
      ~uniforms=uniform,
      ~varying=varying,
      ~vertexShader=vsShader,
      ~fragmentShader=fsShader,
    );
  Shader.compile(shader);
};

