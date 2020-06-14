open Revery;

let _: int = Sys.command("dune build ./examples/Examples.cma");
let basename = "./_build/default/examples/.Examples.objs/byte";
let () =
  List.iter(
    e => Dynlink.loadfile(Filename.concat(basename, e)),
    Sys.readdir(basename)
    |> Array.to_list
    |> List.filter(e => Filename.extension(e) == ".cmo")
    |> List.rev,
  );
//let () = Dynlink.loadfile_private("_build/default/examples/Examples.cma");

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

  /* NOTE: If you want to use network-calls or other IO, uncomment this line */
  // let _startEventLoop = Revery_Lwt.startEventLoop();

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

  let _renderFunction =
    UI.start(window, hook_p^(~window, ~initialExample, ()));
  ();
};

App.start(init);
