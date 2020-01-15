open Reglm;
open Sdl2.Gl;

module DrawContext = {
  type t = {
    canvas: CanvasContext.t,
    pixelRatio: float,
    scaleFactor: float,
    screenWidth: int,
    screenHeight: int,
  };

  let create =
      (~canvas, ~pixelRatio, ~scaleFactor, ~screenWidth, ~screenHeight, ()) => {
    canvas,
    pixelRatio,
    scaleFactor,
    screenWidth,
    screenHeight,
  };
};

let _activeContext: ref(option(DrawContext.t)) = ref(None);

exception NoActiveRenderPassException;

let getContext = () => {
  switch (_activeContext^) {
  | Some(v) => v
  | None => raise(NoActiveRenderPassException)
  };
};

let start =
    (~canvas, ~pixelRatio, ~scaleFactor, ~screenWidth, ~screenHeight, ()) => {
  _activeContext :=
    Some(
      DrawContext.create(
        ~canvas,
        ~pixelRatio,
        ~scaleFactor,
        ~screenWidth,
        ~screenHeight,
        (),
      ),
    );
};

let endAlphaPass = () => {
  _activeContext := None;
};
