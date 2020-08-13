open Skia;

let createSkiaGraphicsContext = (_window: Sdl2.Window.t) => {
  // Issue #759 - first, let's try to create a native context, since that is the most reliable...
  // We'll fall back to an SDL2 context if not available (ie, in Wayland)
  // TODO: There is still something busted with the way GL is being setup, for the SDL strategy not to work!
  // Likely a fix or change is required here: https://github.com/revery-ui/reason-sdl2/blob/94dcd9094534c693998984fd684c642b0f658a43/src/sdl2_wrapper.cpp#L1065
  let interface =
    switch (Skia.Gr.Gl.Interface.makeNative()) {
    | None =>
      print_endline("Unable to create native interface. Falling back to SDL2...");
      Skia.Gr.Gl.Interface.makeSdl2();
    | Some(_) as nativeInterface =>
      nativeInterface;
    };

  switch (Skia.Gr.Context.makeGl(interface)) {
  | None => failwith("Unable to create graphics context")
  | Some(context) => context;
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
//    Printf.printf(
//      "Successfully created canvas: %dx%d\n",
//      framebufferSize.width,
//      framebufferSize.height,
//    );
    surface;
  };
};

type widget = {
  render: (~width: int, ~height: int, Skia.Canvas.t) => unit,
  layout: (~maxWidth: int, ~maxHeight: int, ~children: list(widget)) => (int, int),
  children: list(widget),
};

type window = {
  handle: Sdl2.Window.t,
  id: int,
  widget: widget,
  onResize: option((int, int) => unit),
};

let%nativeComponent window = (~children, ~title, ~width, ~height, ~onResize=?, (), hooks) => (
  {
    make: () => {
      print_endline("make window");
      let handle = Sdl2.Window.create(title, `Undefined, `Undefined, width, height);
      
      Sdl2.Window.center(handle);
      Sdl2.Window.show(handle);
      Sdl2.Window.setResizable(handle, true);
      Sdl2.Window.setMinimumSize(handle, 200, 100);
      
      {
        handle,
        id: Sdl2.Window.getId(handle),
        widget: children,
        onResize
      };
    },
    configureInstance: (~isFirstRender as _, window) => {
      print_endline("configuring window");
      Sdl2.Window.setTitle(window.handle, title);
//      Sdl2.Window.setSize(window.handle, width, height);
      {
        ...window,
        onResize,
      };
    },
    children: Brisk_reconciler.empty,
    insertNode: (~parent, ~child as _, ~position as _) => {
      parent;
    },
    deleteNode: (~parent, ~child as _, ~position as _) => {
      parent;
    },
    moveNode: (~parent, ~child as _, ~from as _, ~to_ as _) => {
      parent;
    },
  },
  hooks,
);

let%nativeComponent widget = (~children, ~layout, ~render=_=>(), (), hooks) => (
  {
    make: () => {
      print_endline("make widget");
      {
        render,
        layout,
        children: [],
      };
    },
    configureInstance: (~isFirstRender as _, widget) => {
      print_endline("configuring widget");
      {
        ...widget,
        render,
        layout,
      };
    },
    children,
    insertNode: (~parent, ~child, ~position as _) => {
      {
        ...parent,
        children: [child, ...parent.children]
      }
    },
    deleteNode: (~parent, ~child as _, ~position as _) => {
      parent;
    },
    moveNode: (~parent, ~child as _, ~from as _, ~to_ as _) => {
      parent;
    },
  },
  hooks,
);

let row = (~children, ()) =>
  <widget
    layout={(~maxWidth, ~maxHeight, ~children) =>
      (maxWidth / List.length(children), maxHeight)}
  />

let box = (~color, ()) =>
  <widget
    layout={(~maxWidth, ~maxHeight, ~children as _) => (maxWidth, maxHeight)}
    render={(~width, ~height, canvas) => {
      let paint = Skia.Paint.make();
      Skia.Paint.setColor(
        paint,
        color,
      );
      Skia.Canvas.drawRectLtwh(canvas, 0., 0., float(width), float(height), paint);

      // text
      Skia.Paint.setColor(
        paint,
        Skia.Color.makeArgb(0xFFl, 0xFFl, 0xFFl, 0xFFl),
      );

      Skia.Canvas.drawText(canvas, "Hello, world!", 50., 50., paint);
    }}
  />

let windows = ref([]);

let renderWindow = window => {
  let glContext = Sdl2.Gl.setup(window.handle);

  Sdl2.Gl.setSwapInterval(1);
  
  Sdl2.Gl.makeCurrent(window.handle, glContext);

  let skiaContext = createSkiaGraphicsContext(window.handle);
  let skiaSurface = createSkiaSurface(window.handle, skiaContext);
  let canvas = Skia.Surface.getCanvas(skiaSurface);
  
  let color = Skia.Color.makeArgb(0xFFl, 0xFFl, 0xF0l, 0x00l);
  Skia.Canvas.clear(canvas, color);

  window.widget.render(canvas);
  Skia.Canvas.flush(canvas);
  Sdl2.Gl.swapWindow(window.handle);
};

type state = {
  width: int,
  height: int,
};

let initialState = {
  width: 800,
  height: 600,
};

let currentState = ref(initialState);
let key = Brisk_reconciler.Key.create();

let app = (~state, ()) =>
  <window
    key
    width=state.width
    height=state.height
    title={Printf.sprintf("Foo %ix%i", state.width, state.height)}
    onResize={(width, height) => currentState := {width, height}}
  >
    <row>
      <box color=Skia.Color.makeArgb(0xFFl, 0xFFl, 0x00l, 0x00l) />
      <box color=Skia.Color.makeArgb(0xFFl, 0x00l, 0xFFl, 0x00l) />
    </row>
  </window>;

let currentElement = ref(<app state=initialState />);

let rendered =
  ref(
    Brisk_reconciler.RenderedElement.render(
      {
        node: (),
        insertNode: (~parent, ~child, ~position as _) => {
          print_endline("insert");
          windows := [child];
          parent;
        },
        deleteNode: (~parent, ~child as _, ~position as _) => {
          print_endline("delete");
          windows := [];
          parent;
        },
        moveNode: (~parent, ~child as _, ~from as _, ~to_ as _) => {
          print_endline("move");
          parent;
        },
      },
      currentElement^,
    )
  );
  
let update = element => {
  rendered :=
    Brisk_reconciler.RenderedElement.update(
      ~previousElement=currentElement^,
      ~renderedElement=rendered^,
      element
    );

  currentElement := element
};

let render = () => {
  Printf.printf("Windows: %i\n%!", List.length(windows^));
  List.iter(renderWindow, windows^);
};
  
Brisk_reconciler.RenderedElement.executeHostViewUpdates(rendered^) |> ignore;
render();

Sdl2.renderLoop(() => {
    switch (Sdl2.Event.wait()) {
    | Ok(Quit) => exit(0)
    | Ok(WindowResized(event)) 
    | Ok(WindowSizeChanged(event)) =>
      switch (List.find_opt(window => window.id == event.windowID, windows^)) {
      | Some(window) =>
        print_endline("resizing");
        Option.iter(f => f(event.width, event.height), window.onResize)
        //let nextElement = Brisk_reconciler.RenderedElement.flushPendingUpdates(rendered^);
        update(<app state=currentState^ />);
        Brisk_reconciler.RenderedElement.executeHostViewUpdates(rendered^) |> ignore;
        render();
        
      | None => ()
      };
    | _ => ()
  };

  false;
});

