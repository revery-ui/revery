open Reglfw;
open Reglfw.Glfw;

type vertexShaderSource = string;
type fragmentShaderSource = string;

module VertexChannel {
    type t = 
    | Position
    | Normal
    | Color
    | TextureCoordinate
    | Custom(string);
}

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


module ShaderAttribute {
    type t = {
        channel: VertexChannel.t,
        dataType: ShaderDataType.t,
        name: string
    };

    let toString = (a) => {
        "attribute " ++ ShaderDataType.toString(a.dataType) ++ " " ++ a.name ++ ";"
    };
}

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

type uncompiledShader = (list(shaderUniform), list(ShaderAttribute.t), list(shaderVarying), vertexShaderSource, fragmentShaderSource); 

let generateAttributeVertexShaderBlock = (attributes: list(ShaderAttribute.t)) => {
    let strs: list(string) = List.map((s) => ShaderAttribute.toString(s), attributes);
    List.fold_left((prev, cur) => prev ++ "\n" ++ cur, "", strs);
};

let create = (~uniforms: list(shaderUniform), ~attributes: list(ShaderAttribute.t), ~varying: list(shaderVarying), ~vertexShader: vertexShaderSource, ~fragmentShader: fragmentShaderSource) => {

    let vertexShader = generateAttributeVertexShaderBlock(attributes) ++ vertexShader;

    (uniforms, attributes, varying, vertexShader, fragmentShader);
};

module CompiledShader {
    type attributeNameToLocation = Hashtbl.t(string, attribLocation);
    type attributeChannelToLocation = Hashtbl.t(VertexChannel.t, attribLocation);

    type t = (list(shaderUniform), list(ShaderAttribute.t), list(shaderVarying), Glfw.program, attributeNameToLocation, attributeChannelToLocation);

    let attributeNameToLocation = (s: t, a: string) => {
        let (_, _, _, _, dict, _) = s;
        Hashtbl.find(dict, a)
    };

    let attributeChannelToLocation = (s: t, a: VertexChannel.t) => {
        let (_, _, _, _, _, dict) = s;
        Hashtbl.find(dict, a);
    };

    let use = (s: t) => {
       let (_, _, _, p, _, _) = s; 
       glUseProgram(p);
    };
}

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
        | LinkSuccess => {
            let attributeNameToLocation: CompiledShader.attributeNameToLocation = Hashtbl.create(List.length(attributes));
            let attributeChannelToLocation: CompiledShader.attributeChannelToLocation = Hashtbl.create(List.length(attributes));

            let addAttributeToHash = (attr: ShaderAttribute.t) => {
                let loc = glGetAttribLocation(program, attr.name);
                print_endline ("adding: " ++ attr.name);
                Hashtbl.add(attributeNameToLocation, attr.name, loc)
                Hashtbl.add(attributeChannelToLocation, attr.channel, loc);
            };

            List.iter(addAttributeToHash, attributes);

            (uniforms, attributes, varying, program, attributeNameToLocation, attributeChannelToLocation);
        }
        | LinkFailure(v) => raise ShaderCompilationException(v);
        }

    };

    switch ((vsResult, fsResult)) {
       | (CompilationSuccess, CompilationSuccess) => linkProgram(vertexShader, fragmentShader)
       | (CompilationFailure(v), CompilationSuccess) => raise ShaderCompilationException(v)
       | (CompilationSuccess, CompilationFailure(v)) => raise ShaderCompilationException(v)
       | (CompilationFailure(v1), CompilationFailure(v2)) => raise ShaderCompilationException(v1 ++ v2)
    }
};

