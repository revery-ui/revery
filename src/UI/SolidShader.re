/**
 * Solid Color Shader
 *
 * Simple shader demonstrating usage of attributes, uniforms, and varying parameters.
 */
open Revery_Shaders;
open Revery_Shaders.Shader;

let attribute: list(ShaderAttribute.t) = [
  {
    dataType: ShaderDataType.Vector2,
    name: "aTexCoord",
    channel: TextureCoordinate,
  },
];

let uniform: list(ShaderUniform.t) = [
  {dataType: ShaderDataType.Vector3, name: "uColor", usage: VertexShader},
  {dataType: ShaderDataType.Vector4, name: "uPosition", usage: VertexShader},
  {dataType: ShaderDataType.Mat4, name: "uWorld", usage: VertexShader},
  {dataType: ShaderDataType.Mat4, name: "uProjection", usage: VertexShader},
];

let varying: list(ShaderVarying.t) = [
  {
    dataType: ShaderDataType.Vector3,
    name: "vColor",
    precision: ShaderPrecision.Low,
  },
];

let vsShader = {|
   vec4 pos = vec4(uPosition.x + (aTexCoord.x * uPosition.z), uPosition.y + (aTexCoord.y * uPosition.w), 1, 1.0);
   gl_Position = uProjection * uWorld * pos;
   vColor = uColor;
|};

let fsShader = {|
    gl_FragColor = vec4(vColor, 1.0);
|};

let create = () => {
  let shader =
    Shader.create(
      ~attributes=attribute,
      ~uniforms=uniform,
      ~varying,
      ~vertexShader=vsShader,
      ~fragmentShader=fsShader,
    );
  Shader.compile(shader);
};
