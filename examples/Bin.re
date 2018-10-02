open Revery;

let init = (app) => {
    let _ = app#createWindow("test");

    let _ = Shader.create([], [], [], "a", "b");

    Lwt.return ();
};

App.start(init);
