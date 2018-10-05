/**
 * Basic Shader
 * 
 * This is the simplest possible shader you can have in Revery -
 * simply takes the vertex position as is, and renders it as a
 * solid color!
 */

let attribute: list(ShaderAttribute.t) = [
{
  dataType: ShaderDataType.Vector3,
  name: "aVertexPosition",
  channel: Position,
},
];

let vsShader = {|
    void main() {
       gl_Position = vec4(aVertexPosition, 1.0);
    }
|};

let fsShader = {|
    void main() {
        gl_FragColor = vec4(1.0);
    }
|};

let create = () => {
  let shader =
    Shader.create(
      ~attributes=attribute,
      ~uniforms=[],
      ~varying=[],
      ~vertexShader=vsShader,
      ~fragmentShader=fsShader,
    );
  Shader.compile(shader);
};
