open Reglm;
open Reglfw.Glfw;

module DrawContext = {
  type t = {
    pixelRatio: float,
    scaleFactor: int,
    screenWidth: int,
    screenHeight: int,
    projection: Mat4.t,
  };

  let create =
      (
        ~pixelRatio,
        ~scaleFactor,
        ~screenWidth,
        ~screenHeight,
        ~projection,
        (),
      ) => {
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

let startAlphaPass =
    (~pixelRatio, ~scaleFactor, ~screenWidth, ~screenHeight, ~projection, ()) => {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  _activeContext :=
    Some(
      DrawContext.create(
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
  glDisable(GL_BLEND);
  _activeContext := None;
};
