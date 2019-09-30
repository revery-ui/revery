open Reglm;
open Sdl2.Gl;

module DrawContext = {
  type t = {
    canvas: Canvas.t,
    pixelRatio: float,
    scaleFactor: float,
    screenWidth: int,
    screenHeight: int,
    projection: Mat4.t,
  };

  let create =
      (
        ~canvas,
        ~pixelRatio,
        ~scaleFactor,
        ~screenWidth,
        ~screenHeight,
        ~projection,
        (),
      ) => {
    canvas,
    pixelRatio,
    scaleFactor,
    screenWidth,
    screenHeight,
    projection,
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

let start=
    (~canvas, ~pixelRatio, ~scaleFactor, ~screenWidth, ~screenHeight, ~projection, ()) => {
  _activeContext :=
    Some(
      DrawContext.create(
        ~canvas, 
        ~pixelRatio,
        ~scaleFactor,
        ~screenWidth,
        ~screenHeight,
        ~projection,
        (),
      ),
    );
};

let endAlphaPass = () => {
  _activeContext := None;
};
