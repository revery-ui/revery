type vertexShaderSource = string;
type fragmentShaderSource = string;

module Environment = Revery_Core.Environment;

module VertexChannel = {
  type t =
    | Position
    | Normal
    | Color
    | TextureCoordinate
    | Custom(string);
};

module ShaderPrecision = {
  type t =
    | Default
    | Low
    | Medium
    | High;

  let toString = v =>
    switch (Environment.webGL, v) {
    | (false, _) => ""
    | (true, p) =>
      switch (p) {
      | Default => ""
      | Low => "lowp"
      | Medium => "mediump"
      | High => "highp"
      }
    };
};

module ShaderDataType = {
  type t =
    | Float
    | Vector2
    | Vector3
    | Vector4
    | Mat4
    | Sampler2D;

  let toString = v =>
    switch (v) {
    | Float => "float"
    | Vector2 => "vec2"
    | Vector3 => "vec3"
    | Vector4 => "vec4"
    | Mat4 => "mat4"
    | Sampler2D => "sampler2D"
    };
};

module ShaderAttribute = {
  type t = {
    channel: VertexChannel.t,
    dataType: ShaderDataType.t,
    name: string,
  };

  let toString = a =>
    "attribute "
    ++ ShaderDataType.toString(a.dataType)
    ++ " "
    ++ a.name
    ++ ";";
};

module ShaderUniform = {
  type shaderUniformUsage =
    | VertexShader
    | FragmentShader
    | VertexAndFragmentShader;

  type t = {
    dataType: ShaderDataType.t,
    usage: shaderUniformUsage,
    name: string,
  };

  let toString = u =>
    "uniform " ++ ShaderDataType.toString(u.dataType) ++ " " ++ u.name ++ ";";
};

module ShaderVarying = {
  type t = {
    dataType: ShaderDataType.t,
    precision: ShaderPrecision.t,
    name: string,
  };

  let toString = v =>
    "varying "
    ++ ShaderPrecision.toString(v.precision)
    ++ " "
    ++ ShaderDataType.toString(v.dataType)
    ++ " "
    ++ v.name
    ++ ";";
};
