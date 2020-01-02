open Revery;
open Lib_view;
open ExampleHost;
let a:
  (~key: Brisk_reconciler.Key.t=?, ~win: Revery.Window.t, unit) =>
  Brisk_reconciler.element(Revery_UI.viewNode) = ExampleHost.make;

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
  //<ExampleHost win />
  let _ignore = UI.start(win, Revery_UI.Hook_p.view^(~win, ()));
  ();
};

let load = {
  let name = Sys.argv[1];
  let name = Filename.concat(name, "default/examples/lib_view.cma");
  let name = Dynlink.adapt_filename(name);
  fun
  | () => {
      let () = print_endline("We will buil :" ++ name);
      let _: int = Sys.command("dune build examples/lib_view.cma");
      let () = print_endline("We will load :" ++ name);
      Dynlink.loadfile_private(name);
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
