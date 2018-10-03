open Revery;

open Revery.Shader;

let init = (app) => {
    let _ = app#createWindow("test");

    let attribute = [{
        dataType: Vector3,
        name: "aVertexPosition",
        channel: Position,
    }];

    let _ = Shader.create(~attributes=attribute, ~uniforms=[], ~varying=[], ~vertexShader="a", ~fragmentShader="b");

    Lwt.return ();
};

App.start(init);
