open Skia;

type element('node) = Brisk_reconciler.element('node);

open {
       let createSkiaGraphicsContext = (_window: Sdl2.Window.t) => {
         // Issue #759 - first, let's try to create a native context, since that is the most reliable...
         // We'll fall back to an SDL2 context if not available (ie, in Wayland)
         // TODO: There is still something busted with the way GL is being setup, for the SDL strategy not to work!
         // Likely a fix or change is required here: https://github.com/revery-ui/reason-sdl2/blob/94dcd9094534c693998984fd684c642b0f658a43/src/sdl2_wrapper.cpp#L1065
         let interface =
           switch (Skia.Gr.Gl.Interface.makeNative()) {
           | None =>
             print_endline(
               "Unable to create native interface. Falling back to SDL2...",
             );
             Skia.Gr.Gl.Interface.makeSdl2();
           | Some(_) as nativeInterface => nativeInterface
           };

         switch (Skia.Gr.Context.makeGl(interface)) {
         | None => failwith("Unable to create graphics context")
         | Some(context) => context
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

         let surfaceProps =
           SurfaceProps.make(Unsigned.UInt32.of_int(0), RgbH);
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
           surface
         };
       };
     };

// WIDGET

module Widget = {
  [@deriving show]
  [@warning "-30"]
  type t = {
    render: (~width: int, ~height: int, Skia.Canvas.t) => unit,
    layout: (~maxWidth: int, ~maxHeight: int, ~children: list(t)) => layout,
    children: list(t),
  }

  and layout = {
    width: int,
    height: int,
    children: list(positionedLayout),
  }

  and positionedLayout = {
    x: int,
    y: int,
    widget: t,
    layout,
  };

  let%nativeComponent make =
                      (
                        ~children=Brisk_reconciler.empty,
                        ~layout,
                        ~render=(~width as _, ~height as _, _) => (),
                        (),
                        hooks,
                      ) => (
    {
      make: () => {
        print_endline("make widget");
        {render, layout, children: []};
      },
      configureInstance: (~isFirstRender as _, widget) => {
        print_endline("configuring widget");
        {...widget, render, layout};
      },
      children,
      insertNode: (~parent, ~child, ~position as _) => {
        {...parent, children: [child, ...parent.children]};
      },
      deleteNode: (~parent, ~child as _, ~position) => {
        {
          ...parent,
          children:
            Base.List.filteri(~f=(i, _) => i != position, parent.children),
        };
      },
      moveNode: (~parent, ~child as _, ~from as _, ~to_ as _) => {
        parent;
      },
    },
    hooks,
  );

  let layout = (~maxWidth, ~maxHeight, widget) => {
    x: 0,
    y: 0,
    layout: widget.layout(~maxWidth, ~maxHeight, ~children=widget.children),
    widget,
  };

  let rec renderLayout = (canvas, layout: positionedLayout) => {
    let transform =
      Skia.Matrix.makeTranslate(float(layout.x), float(layout.y));
    //    let transform = Skia.Matrix.makeTranslate(50., 50.);
    Console.log(layout.x);
    Skia.Canvas.setMatrix(canvas, transform);

    layout.widget.render(
      ~width=layout.layout.width,
      ~height=layout.layout.height,
      canvas,
    );

    Console.log(List.length(layout.layout.children));
    List.iter(renderLayout(canvas), layout.layout.children);
  };
};

// WINDOW

module Window = {
  type t = {
    handle: Sdl2.Window.t,
    id: int,
    widgets: list(Widget.t), // TODO: Should be single
    width: int,
    height: int,
    onResize: option((int, int) => unit),
  };

  let%nativeComponent make =
                      (
                        ~children,
                        ~title,
                        ~width,
                        ~height,
                        ~onResize=?,
                        (),
                        hooks,
                      ) => (
    {
      make: () => {
        print_endline("make window");
        let handle =
          Sdl2.Window.create(title, `Undefined, `Undefined, width, height);

        Sdl2.Window.center(handle);
        Sdl2.Window.show(handle);
        Sdl2.Window.setResizable(handle, true);
        Sdl2.Window.setMinimumSize(handle, 200, 100);

        {
          handle,
          id: Sdl2.Window.getId(handle),
          widgets: [],
          width,
          height,
          onResize,
        };
      },
      configureInstance: (~isFirstRender as _, window) => {
        print_endline("configuring window");
        Sdl2.Window.setTitle(window.handle, title);
        //      Sdl2.Window.setSize(window.handle, width, height);
        {...window, onResize};
      },
      children,
      insertNode: (~parent, ~child, ~position as _) => {
        {...parent, widgets: [child, ...parent.widgets]};
      },
      deleteNode: (~parent, ~child as _, ~position) => {
        {
          ...parent,
          widgets:
            Base.List.filteri(~f=(i, _) => i != position, parent.widgets),
        };
      },
      moveNode: (~parent, ~child as _, ~from as _, ~to_ as _) => {
        parent;
      },
    },
    hooks,
  );

  let render = window => {
    Console.log("renderWindow");
    let glContext = Sdl2.Gl.setup(window.handle);

    Sdl2.Gl.setSwapInterval(1);

    Sdl2.Gl.makeCurrent(window.handle, glContext);

    let skiaContext = createSkiaGraphicsContext(window.handle);
    let skiaSurface = createSkiaSurface(window.handle, skiaContext);
    let canvas = Skia.Surface.getCanvas(skiaSurface);

    Console.log("render background");
    let color = Skia.Color.makeArgb(0xFFl, 0xFFl, 0xFFl, 0xFFl);
    Skia.Canvas.clear(canvas, color);
    //Console.log(List.length(window.widgets));

    List.iter(
      widget => {
        let layout =
          Widget.layout(
            ~maxWidth=window.width,
            ~maxHeight=window.height,
            widget,
          );
        Widget.renderLayout(canvas, layout);
      },
      window.widgets,
    );

    Skia.Canvas.flush(canvas);
    Sdl2.Gl.swapWindow(window.handle);
  };
};
