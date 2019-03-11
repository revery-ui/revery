/**
 * Basic Shader
 *
 * Simple shader demonstrating usage of attributes, uniforms, and varying parameters.
 */
open Types;

let attribute: list(ShaderAttribute.t) = [
  {
    dataType: ShaderDataType.Vector3,
    name: "aVertexPosition",
    channel: Position,
  },
];

let uniform: list(ShaderUniform.t) = [
  {dataType: ShaderDataType.Vector3, name: "uColor", usage: VertexShader},
];

let varying: list(ShaderVarying.t) = [
  {
    dataType: ShaderDataType.Vector3,
    name: "vColor",
    precision: ShaderPrecision.Low,
  },
];

let vsShader = {|
   gl_Position = vec4(aVertexPosition, 1.0);
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
