open {
       type t = Sdl2.Window.nativeWindow;

       external c_setUnsavedWork: (t, bool) => unit =
         "revery_windowSetUnsavedWork";
     };

let setUnsavedWork = (window, truth) =>
  c_setUnsavedWork(window |> Sdl2.Window.getNativeWindow, truth);
