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
           Skia.Gr.Gl.FramebufferInfo.make(
             Unsigned.UInt.of_int(0),
             Unsigned.UInt.of_int(0x8058),
           );

         let framebufferSize = Sdl2.Gl.getDrawableSize(window);
         let backendRenderTarget =
           Skia.Gr.BackendRenderTarget.makeGl(
             framebufferSize.width,
             framebufferSize.height,
             0,
             8,
             framebufferInfo,
           );

         let surfaceProps =
           Skia.SurfaceProps.make(Unsigned.UInt32.of_int(0), RgbH);
         switch (
           Skia.Surface.makeFromBackendRenderTarget(
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

open Primitives;

type t = {
  handle: Sdl2.Window.t,
  id: int,
  onResize: option((int, int) => unit),
};

let create = (~title, ~width, ~height, ~onResize) => {
  //        print_endline("make window");
  let handle =
    Sdl2.Window.create(title, `Undefined, `Undefined, width, height);

  Sdl2.Window.center(handle);
  Sdl2.Window.show(handle);
  Sdl2.Window.setResizable(handle, true);
  Sdl2.Window.setMinimumSize(handle, 200, 100);

  {handle, id: Sdl2.Window.getId(handle), onResize};
};

let render = (element, window) => {
  //    Console.log("renderWindow");
  let glContext = Sdl2.Gl.setup(window.handle);

  Sdl2.Gl.setSwapInterval(1);

  Sdl2.Gl.makeCurrent(window.handle, glContext);

  let skiaContext = createSkiaGraphicsContext(window.handle);
  let skiaSurface = createSkiaSurface(window.handle, skiaContext);
  let canvas = Skia.Surface.getCanvas(skiaSurface);

  //    Console.log("render background");
  let color = Skia.Color.makeArgb(0xFFl, 0xFFl, 0xFFl, 0xFFl);
  Skia.Canvas.clear(canvas, color);
  //Console.log(List.length(window.widgets));

  let Sdl2.Size.{width, height} = Sdl2.Window.getSize(window.handle);

  let constraints =
    BoxConstraints.{
      width: {
        min: 0,
        max: width,
      },
      height: {
        min: 0,
        max: height,
      },
    };

  WidgetElement.performLayout(constraints, element);
  element.position = Some({x: 0, y: 0});
  WidgetElement.render(canvas, element);

  // let layout =
  //   Primitives.Widget.layout(
  //     {minWidth: 0, maxWidth: width, minHeight: 0, maxHeight: height},
  //     widget,
  //   );
  // let layout = {Primitives.Widget.x: 0, y: 0, layout, widget};
  // let layout = Primitives.Widget.globalizeLayoutPositions(layout);
  // Console.log(Primitives.Widget.show_positionedLayout(layout));
  // Primitives.Widget.render(canvas, layout);

  Skia.Canvas.flush(canvas);
  Sdl2.Gl.swapWindow(window.handle);
};
