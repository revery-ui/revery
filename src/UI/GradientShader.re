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

let convertBlurRadius = blur =>
  blur |> string_of_float |> (b => "float amount = " ++ b ++ ";\n");

let fsShader = blur =>
  convertBlurRadius(blur)
  ++ {|
  float leftEdgeAmount = smoothstep(0.0, amount, vTexCoord.x);
  float rightEdgeAmount = smoothstep(0.0, amount, 1.0 - vTexCoord.x);

  float topEdgeAmount = smoothstep(0.0, amount, vTexCoord.y);
  float bottomEdgeAmount = smoothstep(0.0, amount, 1.0 - vTexCoord.y);
  float horizontalBlur = min(leftEdgeAmount, rightEdgeAmount);
  float verticalBlur = min(topEdgeAmount, bottomEdgeAmount);

  float blur = horizontalBlur * verticalBlur;
  gl_FragColor = vec4(1.0, 0.0, 0.0, blur);
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
