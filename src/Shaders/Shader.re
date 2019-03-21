open Reglfw;
open Reglfw.Glfw;
open Reglm;

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

type uncompiledShader = (
  list(ShaderUniform.t),
  list(ShaderAttribute.t),
  list(ShaderVarying.t),
  vertexShaderSource,
  fragmentShaderSource,
);

type blockGenerator('a) = 'a => string;
let generateBlock = (f: blockGenerator('a), l: list('a)) => {
  let strs: list(string) = List.map(f, l);
  List.fold_left((prev, cur) => prev ++ "\n" ++ cur, "", strs);
};

let generateAttributeVertexShaderBlock =
    (attributes: list(ShaderAttribute.t)) =>
  generateBlock(ShaderAttribute.toString, attributes);

let generateUniformVertexShaderBlock = (uniforms: list(ShaderUniform.t)) => {
  let vertexUniforms =
    List.filter((u: ShaderUniform.t) => u.usage != FragmentShader, uniforms);
  generateBlock(ShaderUniform.toString, vertexUniforms);
};

let generateUniformPixelShaderBlock = (uniforms: list(ShaderUniform.t)) => {
  let pixelUniforms =
    List.filter((u: ShaderUniform.t) => u.usage != VertexShader, uniforms);
  generateBlock(ShaderUniform.toString, pixelUniforms);
};

let generateVaryingBlock = (varying: list(ShaderVarying.t)) =>
  generateBlock(ShaderVarying.toString, varying);

let shaderPreamble = Environment.webGL ? "precision mediump float;" : "";

let create =
    (
      ~uniforms: list(ShaderUniform.t),
      ~attributes: list(ShaderAttribute.t),
      ~varying: list(ShaderVarying.t),
      ~vertexShader: vertexShaderSource,
      ~fragmentShader: fragmentShaderSource,
    ) => {
  let vertexShader =
    shaderPreamble
    ++ generateAttributeVertexShaderBlock(attributes)
    ++ generateUniformVertexShaderBlock(uniforms)
    ++ generateVaryingBlock(varying)
    ++ "void main() {\n"
    ++ vertexShader
    ++ "}\n";

  let fragmentShader =
    shaderPreamble
    ++ generateUniformPixelShaderBlock(uniforms)
    ++ generateVaryingBlock(varying)
    ++ "void main() {\n"
    ++ fragmentShader
    ++ "}\n";

  (uniforms, attributes, varying, vertexShader, fragmentShader);
};

module CompiledShader = {
  type attributeNameToLocation = Hashtbl.t(string, attribLocation);
  type attributeChannelToLocation =
    Hashtbl.t(VertexChannel.t, attribLocation);
  type uniformNameToLocation = Hashtbl.t(string, uniformLocation);

  type t = (
    list(ShaderUniform.t),
    list(ShaderAttribute.t),
    list(ShaderVarying.t),
    Glfw.program,
    attributeNameToLocation,
    attributeChannelToLocation,
    uniformNameToLocation,
  );

  let attributeNameToLocation = (s: t, a: string) => {
    let (_, _, _, _, dict, _, _) = s;
    Hashtbl.find_opt(dict, a);
  };

  let attributeChannelToLocation = (s: t, a: VertexChannel.t) => {
    let (_, _, _, _, _, dict, _) = s;
    Hashtbl.find_opt(dict, a);
  };

  let uniformNameToLocation = (s: t, name: string) => {
    let (_, _, _, _, _, _, u) = s;
    Hashtbl.find_opt(u, name);
  };

  let _setUniformIfAvailable = (s, name, f) => {
    let uLoc = uniformNameToLocation(s, name);
    switch (uLoc) {
    | Some(u) => f(u)
    | None => ()
    };
  };

  let setUniform2fv = (s: t, name: string, v: Vec2.t) =>
    _setUniformIfAvailable(s, name, u => glUniform2fv(u, v));

  let setUniform3fv = (s: t, name: string, v: Vec3.t) =>
    _setUniformIfAvailable(s, name, u => glUniform3fv(u, v));

  let setUniform1f = (s: t, name: string, f: Float.t) =>
    _setUniformIfAvailable(s, name, u => glUniform1f(u, f));
  
  let setUniform4f =
      (s: t, name: string, x: float, y: float, z: float, w: float) =>
    _setUniformIfAvailable(s, name, u => glUniform4f(u, x, y, z, w));

  let setUniform4fv = (s: t, name: string, v: Vec4.t) =>
    _setUniformIfAvailable(s, name, u => glUniform4fv(u, v));

  let setUniformMatrix4fv = (s: t, name: string, m: Mat4.t) =>
    _setUniformIfAvailable(s, name, u => glUniformMatrix4fv(u, m));

  let use = (s: t) => {
    let (_, _, _, p, _, _, _) = s;
    glUseProgram(p);
  };
};

exception ShaderCompilationException(string);

let compile = (shader: uncompiledShader) => {
  let (uniforms, attributes, varying, vs, fs) = shader;

  let vertexShader = glCreateShader(Glfw.GL_VERTEX_SHADER);
  let fragmentShader = glCreateShader(Glfw.GL_FRAGMENT_SHADER);

  let () = glShaderSource(vertexShader, vs);
  let () = glShaderSource(fragmentShader, fs);

  let vsResult = glCompileShader(vertexShader);
  let fsResult = glCompileShader(fragmentShader);

  let linkProgram = (vs, fs) => {
    let program = glCreateProgram();
    let () = glAttachShader(program, vs);
    let () = glAttachShader(program, fs);

    let result = glLinkProgram(program);

    switch (result) {
    | LinkSuccess =>
      let attributeNameToLocation: CompiledShader.attributeNameToLocation =
        Hashtbl.create(List.length(attributes));
      let attributeChannelToLocation: CompiledShader.attributeChannelToLocation =
        Hashtbl.create(List.length(attributes));

      let uniformNameToLocation: CompiledShader.uniformNameToLocation =
        Hashtbl.create(List.length(uniforms));

      let addAttributeToHash = (attr: ShaderAttribute.t) => {
        let loc = glGetAttribLocation(program, attr.name);
        Hashtbl.add(attributeNameToLocation, attr.name, loc);
        Hashtbl.add(attributeChannelToLocation, attr.channel, loc);
      };

      let addUniformToHash = (uniform: ShaderUniform.t) => {
        let uloc = glGetUniformLocation(program, uniform.name);
        Hashtbl.add(uniformNameToLocation, uniform.name, uloc);
      };

      List.iter(addAttributeToHash, attributes);
      List.iter(addUniformToHash, uniforms);

      (
        uniforms,
        attributes,
        varying,
        program,
        attributeNameToLocation,
        attributeChannelToLocation,
        uniformNameToLocation,
      );
    | LinkFailure(v) => raise(ShaderCompilationException(v))
    };
  };

  switch (vsResult, fsResult) {
  | (CompilationSuccess, CompilationSuccess) =>
    linkProgram(vertexShader, fragmentShader)
  | (CompilationFailure(v), CompilationSuccess) =>
    raise(ShaderCompilationException(v))
  | (CompilationSuccess, CompilationFailure(v)) =>
    raise(ShaderCompilationException(v))
  | (CompilationFailure(v1), CompilationFailure(v2)) =>
    raise(ShaderCompilationException(v1 ++ v2))
  };
};
