open Revery;
open Lib_view;
/*
open ExampleHost;
let a:
  (~key: Brisk_reconciler.Key.t=?, ~window: Revery.Window.t, ~initialExample: string, ~setGen: 'a, unit) =>
  Brisk_reconciler.element(Revery_UI.viewNode) = ExampleHost.make;
*/
let _ignore = (ExampleHost.ExampleHost.make);

module ExampleHost = {
  let%component make = (~window, ~initialExample, ()) => {
    let%hook (_, setGen) = UI.Hooks.state(UI.Hook_p.gen^);
    (Revery_UI.Hook_p.view^(~window, ~initialExample, ~setGen, ()));
  }
}

let init = app => {
  Revery.App.initConsole();

  Timber.App.enable();
  Timber.App.setLevel(Timber.Level.perf);

  App.onIdle(app, () => prerr_endline("Idle!"))
  |> (ignore: Revery.App.unsubscribe => unit);
  App.onBeforeQuit(app, () => prerr_endline("Quitting!"))
  |> (ignore: Revery.App.unsubscribe => unit);

  let initialExample = ref("Animation");
  let decorated = ref(true);
  let forceScaleFactor = ref(None);
  Arg.parse(
    [
      ("--trace", Unit(() => Timber.App.setLevel(Timber.Level.trace)), ""),
      ("--no-decoration", Unit(() => decorated := false), ""),
      ("--example", String(name => initialExample := name), ""),
      (
        "--force-device-scale-factor",
        Float(scaleFactor => forceScaleFactor := Some(scaleFactor)),
        "",
      ),
    ],
    _ => (),
    "There is only --trace, --example, --no-decoration, and --force-device-scale-factor",
  );
  let initialExample = initialExample^;

  let maximized = Environment.webGL;

  let windowWidth = 800;
  let windowHeight = 480;

  Console.log("Hello from example app");
  Console.log([1, 2, 3]);

  let window =
    App.createWindow(
      ~createOptions=
        WindowCreateOptions.create(
          ~width=windowWidth,
          ~height=windowHeight,
          ~maximized,
          ~titlebarStyle=Transparent,
          ~icon=Some("revery-icon.png"),
          ~decorated=decorated^,
          ~forceScaleFactor=forceScaleFactor^,
          (),
        ),
      app,
      "Welcome to Revery!",
    );

  if (Environment.webGL) {
    Window.maximize(window);
  };

  let _unsubscribe =
    Window.onFocusGained(window, () => Console.log("Focus gained"));
  let _unsubscribe =
    Window.onFocusLost(window, () => Console.log("Focus lost"));
  let _unsubscribe =
    Window.onMaximized(window, () => Console.log("Maximized!"));
  let _unsubscribe =
    Window.onFullscreen(window, () => Console.log("Fullscreen!"));
  let _unsubscribe =
    Window.onMinimized(window, () => Console.log("Minimized!"));
  let _unsubscribe =
    Window.onRestored(window, () => Console.log("Restored!"));

  let _unsubscribe =
    Window.onSizeChanged(window, ({width, height}) =>
      Console.log(Printf.sprintf("Size changed: %d x %d", width, height))
    );

  let _unsubscribe =
    Window.onMoved(window, ((x, y)) =>
      Console.log(Printf.sprintf("Moved: %d x %d", x, y))
    );

  let _renderFunction = UI.start(window, <ExampleHost window initialExample />);
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
App.start(/*~onIdle, */init);
