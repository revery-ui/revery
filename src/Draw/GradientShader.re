/**
 * Gradient Shader
 *
 * shader used for specificying a gradient.
 * the gradient is used to draw a box shadow
 */
open Reglfw.Glfw;
open Revery_Shaders;

let attributes: list(ShaderAttribute.t) =
  SolidShader.attribute
  @ [
    {
      dataType: ShaderDataType.Vector2,
      name: "aTexCoord",
      channel: TextureCoordinate,
    },
  ];

let uniforms: list(ShaderUniform.t) =
  SolidShader.uniform
  @ [
    {
      dataType: ShaderDataType.Sampler2D,
      name: "uSampler",
      usage: FragmentShader,
    },
    {
      dataType: ShaderDataType.Vector3,
      name: "uShadowColor",
      usage: FragmentShader,
    },
    {
      dataType: ShaderDataType.Vector2,
      name: "uShadowAmount",
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

let vertexShader =
  SolidShader.vsShader ++ "\n" ++ {|
  vTexCoord = aTexCoord;
|};

let fragmentShader = {|
  float leftEdgeAmount = smoothstep(0.0, uShadowAmount.x, vTexCoord.x);
  float rightEdgeAmount = smoothstep(0.0, uShadowAmount.x, 1.0 - vTexCoord.x);

  float topEdgeAmount = smoothstep(0.0, uShadowAmount.y, vTexCoord.y);
  float bottomEdgeAmount = smoothstep(0.0, uShadowAmount.y, 1.0 - vTexCoord.y);
  float horizontalBlur = min(leftEdgeAmount, rightEdgeAmount);
  float verticalBlur = min(topEdgeAmount, bottomEdgeAmount);

  float blur = horizontalBlur * verticalBlur;
  gl_FragColor = vec4(uShadowColor, blur);
|};

type t = {
  compiledShader: CompiledShader.t,
  uniformProjection: uniformLocation,
  uniformShadowColor: uniformLocation,
  uniformShadowAmount: uniformLocation,
  uniformWorld: uniformLocation,
};

let create = () => {
  let shader =
    Shader.create(
      ~attributes,
      ~varying,
      ~uniforms,
      ~vertexShader,
      ~fragmentShader,
    );
  let compiledShader = Shader.compile(shader);
  let uniformWorld =
    CompiledShader.getUniformLocation(compiledShader, "uWorld");
  let uniformProjection =
    CompiledShader.getUniformLocation(compiledShader, "uProjection");
  let uniformShadowColor =
    CompiledShader.getUniformLocation(compiledShader, "uShadowColor");
  let uniformShadowAmount =
    CompiledShader.getUniformLocation(compiledShader, "uShadowAmount");

  {
    compiledShader,
    uniformWorld,
    uniformProjection,
    uniformShadowColor,
    uniformShadowAmount,
  };
};
