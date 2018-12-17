/**
 * Gradient Shader
 *
 * shader demonstrating which allows for specificying a gradient.
 */
open Revery_Shaders;
open Revery_Shaders.Shader;

let attributes: list(ShaderAttribute.t) =
  SolidShader.attribute
  @ [
    {
      dataType: ShaderDataType.Vector2,
      name: "aTexCoord",
      channel: TextureCoordinate,
    },
  ];

let uniform: list(ShaderUniform.t) =
  SolidShader.uniform
  @ [
    {
      dataType: ShaderDataType.Sampler2D,
      name: "uSampler",
      usage: FragmentShader,
    },
  ];

let varying =
  SolidShader.varying
  @ [
    {
      dataType: ShaderDataType.Vector2,
      name: "vTexCoord",
      precision: ShaderPrecision.Low,
    },
  ];

let vsShader = SolidShader.vsShader ++ "\n" ++ {|
  vTexCoord = aTexCoord;
|};

let fsShader = _blur => {|
  float grad = vTexCoord.x / 0.1;
  if (vTexCoord.x < 0.1) {
    gl_FragColor = vec4(0.0, 0.0, 0.0, grad * grad);
  } else if (vTexCoord.y < 0.1) {
    gl_FragColor = vec4(0.0, 0.0, 0.0, grad * grad);
  } else {
    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
  }
|};

let create = blur => {
  let shader =
    Shader.create(
      ~attributes,
      ~varying,
      ~uniforms=uniform,
      ~vertexShader=vsShader,
      ~fragmentShader=fsShader(blur),
    );
  Shader.compile(shader);
};
