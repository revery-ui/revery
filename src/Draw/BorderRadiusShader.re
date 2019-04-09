/**
 * Solid Color Shader
 *
 * Simple shader demonstrating usage of attributes, uniforms, and varying parameters.
 */
open Reglfw.Glfw;
open Revery_Shaders;

let attribute: list(ShaderAttribute.t) = [
  {dataType: ShaderDataType.Vector2, name: "aPosition", channel: Position},
];

let uniform: list(ShaderUniform.t) = [
  {dataType: ShaderDataType.Vector4, name: "uColor", usage: VertexShader},
  {dataType: ShaderDataType.Mat4, name: "uWorld", usage: VertexShader},
  {dataType: ShaderDataType.Mat4, name: "uProjection", usage: VertexShader},
  {
    dataType: ShaderDataType.Vector2,
    name: "uResolution",
    usage: FragmentShader,
  },
  {
    dataType: ShaderDataType.Float,
    name: "uBorderRadius",
    usage: FragmentShader,
  },
];

let varying: list(ShaderVarying.t) = [
  {
    dataType: ShaderDataType.Vector4,
    name: "vColor",
    precision: ShaderPrecision.Low,
  },
  {
    dataType: ShaderDataType.Vector2,
    name: "vPosition",
    precision: ShaderPrecision.High,
  },
];

let vsShader = {|
  vec4 pos = vec4(aPosition.x, aPosition.y, 1.0, 1.0);
  gl_Position = uProjection * uWorld * pos;
  vColor = uColor;
  vPosition = aPosition;
|};

let fsShader = {|
  vec2 halfRes = 0.5 * uResolution.xy;
  float box = length(max(abs(vPosition - halfRes) - halfRes + uBorderRadius, 0.0)) - uBorderRadius;
  float delta = fwidth(box);
  float alpha = smoothstep(0.45-delta, 0.45, box);

  gl_FragColor = vec4(vColor.xyz, 1.0 - alpha);
|};

type t = {
  compiledShader: CompiledShader.t,
  uniformWorld: uniformLocation,
  uniformProjection: uniformLocation,
  uniformResolution: uniformLocation,
  uniformColor: uniformLocation,
  uniformBorderRadius: uniformLocation,
};

let create = () => {
  let shader =
    Shader.create(
      ~attributes=attribute,
      ~uniforms=uniform,
      ~varying,
      ~vertexShader=vsShader,
      ~fragmentShader=fsShader,
    );
  let compiledShader = Shader.compile(shader);

  {
    compiledShader,
    uniformWorld: CompiledShader.getUniformLocation(compiledShader, "uWorld"),
    uniformProjection:
      CompiledShader.getUniformLocation(compiledShader, "uProjection"),
    uniformResolution:
      CompiledShader.getUniformLocation(compiledShader, "uResolution"),
    uniformColor: CompiledShader.getUniformLocation(compiledShader, "uColor"),
    uniformBorderRadius:
      CompiledShader.getUniformLocation(compiledShader, "uBorderRadius"),
  };
};
