/**
 * TextureShader
 *
 * Simple texture shader
 */
open Sdl2.Gl;
open Revery_Shaders;

let attribute: list(ShaderAttribute.t) =
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

let fsShader = {|
    gl_FragColor = vColor * texture2D(uSampler, vTexCoord);
|};

type t = {
  compiledShader: CompiledShader.t,
  uniformLocal: uniformLocation,
  uniformWorld: uniformLocation,
  uniformProjection: uniformLocation,
  uniformColor: uniformLocation,
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
  let uniformLocal =
    CompiledShader.getUniformLocation(compiledShader, "uLocal");
  let uniformWorld =
    CompiledShader.getUniformLocation(compiledShader, "uWorld");
  let uniformProjection =
    CompiledShader.getUniformLocation(compiledShader, "uProjection");
  let uniformColor =
    CompiledShader.getUniformLocation(compiledShader, "uColor");

  {compiledShader, uniformLocal, uniformWorld, uniformProjection, uniformColor};
};
