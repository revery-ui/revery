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

let updateShader = (~blur, ~height, ~width, ~color) => {
  let amountX = string_of_float(blur /. width);
  let amountY = string_of_float(blur /. height);
  open Color.Rgba';
  let colorStr =
    "vec3 uShadowColor = "
    ++ "vec3("
    ++ string_of_float(color.r)
    ++ ","
    ++ string_of_float(color.g)
    ++ ","
    ++ string_of_float(color.b)
    ++ ")"
    ++ ";\n";

  let amountStr =
    "float amountX = "
    ++ amountX
    ++ ";\n"
    ++ "float amountY = "
    ++ amountY
    ++ ";\n";
  amountStr ++ colorStr;
};

let createFragmentShader = (~blur, ~height, ~width, ~color) =>
  updateShader(~blur, ~height, ~width, ~color)
  ++ {|
  float leftEdgeAmount = smoothstep(0.0, amountX, vTexCoord.x);
  float rightEdgeAmount = smoothstep(0.0, amountX, 1.0 - vTexCoord.x);

  float topEdgeAmount = smoothstep(0.0, amountY, vTexCoord.y);
  float bottomEdgeAmount = smoothstep(0.0, amountY, 1.0 - vTexCoord.y);
  float horizontalBlur = min(leftEdgeAmount, rightEdgeAmount);
  float verticalBlur = min(topEdgeAmount, bottomEdgeAmount);

  float blur = horizontalBlur * verticalBlur;
  gl_FragColor = vec4(uShadowColor, blur);
|};

let create = (~blur, ~height, ~width, ~color) => {
  let shader =
    Shader.create(
      ~attributes,
      ~varying,
      ~uniforms=uniform,
      ~vertexShader=vsShader,
      ~fragmentShader=createFragmentShader(~blur, ~height, ~width, ~color),
    );
  Shader.compile(shader);
};
