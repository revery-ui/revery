open Revery;
open Lib_view;
/*
open ExampleHost;
let a:
  (~key: Brisk_reconciler.Key.t=?, ~win: Revery.Window.t, ~setGen: 'a, unit) =>
  Brisk_reconciler.element(Revery_UI.viewNode) = ExampleHost.make;
*/
let _ignore = (ExampleHost.ExampleHost.make);

module ExampleHost = {
  let%component make = (~win, ()) => {
    let%hook (_, setGen) = UI.Hooks.state(UI.Hook_p.gen^);
    (Revery_UI.Hook_p.view^(~win, ~setGen, ()));
  }
}

let init = app => {
  let _ignore = Log.listen((_, msg) => print_endline(msg));

  let maximized = Environment.webGL;

  let windowWidth = 800;
  let windowHeight = 480;

  Console.log("Hello from example app");
  Console.log([1, 2, 3]);

  let win =
    App.createWindow(
      ~createOptions=
        WindowCreateOptions.create(
          ~width=windowWidth,
          ~height=windowHeight,
          ~maximized,
          ~icon=Some("revery-icon.png"),
          (),
        ),
      app,
      "Welcome to Revery!",
    );

  if (Environment.webGL) {
    Window.maximize(win);
    ();
  } else {
    Window.center(win);
    ();
  };
  let _ignore = UI.start(win, <ExampleHost win />);
  ();
};

let load = {
  let filename =
    if (Sys.file_exists("_build") && Sys.is_directory("_build")) {
      "_build";
    } else if (Array.length(Sys.argv) > 1) {
      Sys.argv[1];
    } else {
      Sys.getcwd();
    };
  let filename = Filename.concat(filename, "default");
  let name = Dynlink.adapt_filename("examples/lib_view.cma");
  let filename = Filename.concat(filename, name);
  fun
  | () => {
      let () = print_endline("We will build :" ++ filename);
      let start = Unix.gettimeofday();
      let _: int = Sys.command("dune build " ++ name);
      let end_ = Unix.gettimeofday();
      let () = Printf.printf("Builded in %f seconds\n%!", end_ -. start);
      let () = print_endline("We will load :" ++ filename);
      Dynlink.loadfile_private(filename);
      /*let () =
        try(Dynlink.loadfile_private(name)) {
        | Dynlink.Error(
            Dynlink.Cannot_open_dynamic_library(
              Dynlink.Error(
                Dynlink.Cannot_open_dynamic_library(Failure(error)),
              ),
            ),
          ) =>
          let () = print_endline(error);
          exit(0);
        };*/
      print_endline("We have loaded :" ++ name);
    };
};

load();

let onIdle = () => {
  load();
  print_endline("Example: idle callback triggered");
};
App.start(~onIdle, init);
