open Reglfw;
open Reglfw.Glfw;

type vertexShaderSource = string;
type fragmentShaderSource = string;

type vertexChannel = 
| Position
| Normal
| Color
| TextureCoordinate;

type shaderPrecision =
| Default
| Low
| Medium
| High;

type shaderDataType =
| Vector2
| Vector3
| Vector4
| Mat4
| Sampler2D;

type shaderUsage =
| VertexShader
| FragmentShader
| VertexAndFragmentShader

type shaderAttribute = {
    channel: vertexChannel,
    dataType: shaderDataType,
    name: string
};

type shaderUniform = {
    dataType: shaderDataType,
    usage: shaderUsage,
    name: string
};

type shaderVarying = {
    dataType: shaderDataType,
    precision: shaderPrecision,
    name: string
};

type uncompiledShader = (list(shaderUniform), list(shaderAttribute), list(shaderVarying), vertexShaderSource, fragmentShaderSource); 
type compiledShader = (list(shaderUniform), list(shaderAttribute), list(shaderVarying), Glfw.program);

let create = (uniforms: list(shaderUniform), attributes: list(shaderAttribute), varying: list(shaderVarying), vertexShaderSource: vertexShaderSource, fragmentShaderSource: fragmentShaderSource) => {
    (uniforms, attributes, varying, vertexShaderSource, fragmentShaderSource);
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
        let _ = glAttachShader(program, vs);
        let _ = glAttachShader(program, fs);
        /* TODO: Errors! */
        let _ = glLinkProgram(program);

        ShaderCompilationSuccess((uniforms, attributes, varying, program));
    };

    switch ((vsResult, fsResult)) {
       | (CompilationSuccess, CompilationSuccess) => linkProgram(vertexShader, fragmentShader)
       | (CompilationFailure(v), CompilationSuccess) => ShaderCompilationFailure(v)
       | (CompilationSuccess, CompilationFailure(v)) => ShaderCompilationFailure(v)
       | (CompilationFailure(v1), CompilationFailure(v2)) => ShaderCompilationFailure(v1 ++ v2)
    }
};
