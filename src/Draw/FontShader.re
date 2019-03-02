/**
 * TextureShader
 *
 * Simple texture shader
 */
open Revery_Shaders;
open Revery_Shaders.Shader;

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
    {
      dataType: ShaderDataType.Vector4,
      name: "uBackgroundColor",
      usage: FragmentShader,
    },
    {dataType: ShaderDataType.Float, name: "uGamma", usage: FragmentShader},
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

/*
 * DefaultShader
 *
 * This is the shader used when there is no background or a transparent background.
 * It does not properly handle gamma correction or subpixel anti-aliasing,
 * so the text render quality will be lower with this case.
 *
 * However, it is the fastest and most convenient text rendering-strategy.
 */
module Default = {
  let fsShader = {|
        vec4 t = texture2D(uSampler, vTexCoord);
        gl_FragColor = vec4(vColor.r, vColor.g, vColor.b, t.a * vColor.a);
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
};

module GammaCorrected = {
  let fsShader = {|
        vec4 t = texture2D(uSampler, vTexCoord);
        vec4 alpha = t.a;
        vec4 fg = vColor;
        vec4 bg = uBackgroundColor;

        float GAMMA = uGamma;
        float INV_GAMMA = 1.0 / GAMMA;

        float r0 = pow(fg.r, GAMMA) * alpha + pow(bg.r, GAMMA) * (1.0 - alpha);
        float g0 = pow(fg.g, GAMMA) * alpha + pow(bg.g, GAMMA) * (1.0 - alpha);
        float b0 = pow(fg.b, GAMMA) * alpha + pow(bg.b, GAMMA) * (1.0 - alpha);

        float r = pow(r0, INV_GAMMA);
        float g = pow(g0, INV_GAMMA);
        float b = pow(b0, INV_GAMMA);

        gl_FragColor = vec4(r, g, b, 1.0);
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
};
