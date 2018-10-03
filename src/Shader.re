open Reglfw;
open Reglfw.Glfw;

type vertexShaderSource = string;
type fragmentShaderSource = string;

type vertexChannel = 
| Position
| Normal
| Color
| TextureCoordinate;

module ShaderPrecision {
    type t =
    | Default
    | Low
    | Medium
    | High;

    let toString = (v) => {
        switch((Environment.webGL, v)) {
        | (false, _) => ""
        | (true, p) => switch(p) {
            | Default => ""
            | Low => "lowp"
            | Medium => "mediump"
            | High => "highp"
            };
        };
    };
}


module ShaderDataType {
    type t =
    | Float
    | Vector2
    | Vector3
    | Vector4
    | Mat4
    | Sampler2D;

    let toString = (v) => {
        switch(v) {
        | Float => "float"
        | Vector2 => "vec2"
        | Vector3 => "vec3"
        | Vector4 => "vec4"
        | Mat4 => "mat4"
        | Sampler2D => "sampler2D"
        };
    };
}

type shaderUniformUsage =
| VertexShader
| FragmentShader
| VertexAndFragmentShader

type shaderAttribute = {
    channel: vertexChannel,
    dataType: ShaderDataType.t,
    name: string
};

type shaderUniform = {
    dataType: ShaderDataType.t,
    usage: shaderUniformUsage,
    name: string
};

type shaderVarying = {
    dataType: ShaderDataType.t,
    precision: ShaderPrecision.t,
    name: string
};

type uncompiledShader = (list(shaderUniform), list(shaderAttribute), list(shaderVarying), vertexShaderSource, fragmentShaderSource); 
type compiledShader = (list(shaderUniform), list(shaderAttribute), list(shaderVarying), Glfw.program);

let create = (~uniforms: list(shaderUniform), ~attributes: list(shaderAttribute), ~varying: list(shaderVarying), ~vertexShader: vertexShaderSource, ~fragmentShader: fragmentShaderSource) => {
    (uniforms, attributes, varying, vertexShader, fragmentShader);
};

type shaderCompilationResult = 
| ShaderCompilationSuccess(compiledShader)
| ShaderCompilationFailure(string);

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
        /* TODO: Errors! */
        let result = glLinkProgram(program);

        switch (result) {
        | LinkSuccess => ShaderCompilationSuccess((uniforms, attributes, varying, program))
        | LinkFailure(v) => ShaderCompilationFailure(v)
        }

    };

    switch ((vsResult, fsResult)) {
       | (CompilationSuccess, CompilationSuccess) => linkProgram(vertexShader, fragmentShader)
       | (CompilationFailure(v), CompilationSuccess) => ShaderCompilationFailure(v)
       | (CompilationSuccess, CompilationFailure(v)) => ShaderCompilationFailure(v)
       | (CompilationFailure(v1), CompilationFailure(v2)) => ShaderCompilationFailure(v1 ++ v2)
    }
};
