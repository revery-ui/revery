open Reglfw;
open Reglfw.Glfw;

type vertexShaderSource = string;
type fragmentShaderSource = string;

module Environment = Revery_Core.Environment;

open Types;

type t = (
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

exception ShaderCompilationException(string);

let compile = (shader: t) => {
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
