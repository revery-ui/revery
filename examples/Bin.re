open Reglfw.Glfw;
open Revery;

open Revery.Shader;

let init = (app) => {
    print_endline ("init");
    let w = app#createWindow("test");

    w#setRenderCallback(() => {
        glClearColor(1.0, 0.0, 0.0, 1.0);
    });

    print_endline ("after window");

    let attribute: list(ShaderAttribute.t) = [{
        dataType: ShaderDataType.Vector3,
        name: "aVertexPosition",
        channel: Position,
    }];

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

    let shader = Shader.create(~attributes=attribute, ~uniforms=[], ~varying=[], ~vertexShader=vsShader, ~fragmentShader=fsShader);
    print_endline ("shader create");
    let result = Shader.compile(shader);
    print_endline ("shader compile");

    switch (result) {
    | ShaderCompilationSuccess(_) => print_endline ("Shader compiled successfully!")
    | ShaderCompilationFailure(s) => print_endline ("Shader compilation failed: " ++ s);
    }

    Lwt.return ();
};

App.start(init);
