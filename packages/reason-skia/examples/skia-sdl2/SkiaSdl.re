open Skia;

let ctx = ref(None);

let printEnv = env =>
  switch (Sys.getenv_opt(env)) {
  | None => print_endline(env ++ " : not defined ")
  | Some(v) => print_endline(env ++ " : " ++ v)
  };

printEnv("WAYLAND_DISPLAY");
printEnv("XDG_SESSION_TYPE");

let createSkiaGraphicsContext = (_window: Sdl2.Window.t) => {
  print_endline("Creating graphics context");
  let nativeInterface = Skia.Gr.Gl.Interface.makeNative();
  switch (nativeInterface) {
  | Some(_) => print_endline("Native interface created successfully.")
  | None => print_endline("Native interface is null")
  };

  let interface = Skia.Gr.Gl.Interface.(Sys.win32 ? makeSdl2ES() : makeSdl2());
  print_endline("Have interface...");
  let context = Skia.Gr.Context.makeGl(interface);

  switch (context) {
  | None => failwith("Unable to create graphics context")
  | Some(glContext) =>
    print_endline("glContext created successfully");
    ctx := Some(glContext);
    glContext;
  };
};

let createSkiaSurface =
    (window: Sdl2.Window.t, skiaContext: Skia.Gr.Context.t) => {
  let framebufferInfo =
    Gr.Gl.FramebufferInfo.make(
      Unsigned.UInt.of_int(0),
      Unsigned.UInt.of_int(0x8058),
    );

  let framebufferSize = Sdl2.Gl.getDrawableSize(window);
  let backendRenderTarget =
    Gr.BackendRenderTarget.makeGl(
      framebufferSize.width,
      framebufferSize.height,
      0,
      8,
      framebufferInfo,
    );

  let surfaceProps = SurfaceProps.make(Unsigned.UInt32.of_int(0), RgbH);
  switch (
    Surface.makeFromBackendRenderTarget(
      skiaContext,
      backendRenderTarget,
      BottomLeft,
      Rgba8888,
      None,
      Some(surfaceProps),
    )
  ) {
  | None => failwith("Unable to create skia surface.")
  | Some(surface) =>
    Printf.printf(
      "Successfully created canvas: %dx%d\n",
      framebufferSize.width,
      framebufferSize.height,
    );
    surface;
  };
};

let run = () => {
  let _ = Sdl2.init();
  let attachResult = Sdl2.Platform.win32AttachConsole();

  // If we were unable to attach a console, try allocating a new one
  let _code =
    if (attachResult == 0) {
      Sdl2.Platform.win32AllocConsole();
    } else {
      attachResult;
    };

  let primaryWindow =
    Sdl2.Window.create("test", `Undefined, `Undefined, 100, 100, `Auto);
  let glContext = Sdl2.Gl.setup(primaryWindow);

  Sdl2.Gl.setSwapInterval(1);

  Sdl2.Window.setTitle(primaryWindow, "reason-skia-sdl2 example");
  Sdl2.Window.setWin32ProcessDPIAware(primaryWindow);

  let _scale = Sdl2.Window.getWin32ScaleFactor(primaryWindow);
  let _display = Sdl2.Window.getDisplay(primaryWindow);

  Sdl2.Window.setSize(primaryWindow, 800, 600);
  Sdl2.Window.center(primaryWindow);

  Sdl2.Window.show(primaryWindow);

  Sdl2.Window.setResizable(primaryWindow, true);
  Sdl2.Window.setMinimumSize(primaryWindow, 200, 100);
  Sdl2.Gl.makeCurrent(primaryWindow, glContext);

  let skiaContext = createSkiaGraphicsContext(primaryWindow);
  let skiaSurface = createSkiaSurface(primaryWindow, skiaContext);
  let canvas = Skia.Surface.getCanvas(skiaSurface);

  let render = (window, context, surface) => {
    //print_endline("-- Render: start");
    ignore(context);
    ignore(surface);

    let color = Skia.Color.makeArgb(0xFFl, 0xFFl, 0x00l, 0x00l);

    Skia.Canvas.clear(canvas, color);

    let paint = Skia.Paint.make();
    Skia.Paint.setColor(
      paint,
      Skia.Color.makeArgb(0xFFl, 0xFFl, 0xFFl, 0xFFl),
    );

    Skia.Canvas.drawText(canvas, "Hello, world!", 50., 50., paint);

    Skia.Canvas.flush(canvas);
    Sdl2.Gl.swapWindow(window);
    //print_endline("-- Render: end");
  };

  //let frame = ref(0);
  Sdl2.renderLoop(() => {
    switch (Sdl2.Event.poll()) {
    | None => ()
    | Some(evt) =>
      switch (evt) {
      | Sdl2.Event.Quit => exit(0)
      | _ => ()
      }
    };

    render(primaryWindow, skiaContext, skiaSurface);
    Gc.full_major();
    false;
  });

  Lwt.return();
};

run();
