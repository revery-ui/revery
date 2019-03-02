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

type t =
  | SolidPass(DrawContext.t)
  | AlphaPass(DrawContext.t);

let _activePass: ref(option(t)) = ref(None);

exception NoActiveRenderPassException;

let getCurrent = () => {
  switch (_activePass^) {
  | Some(v) => v
  | None => raise(NoActiveRenderPassException)
  };
};

let getContext = () =>
  switch (getCurrent()) {
  | SolidPass(v) => v
  | AlphaPass(v) => v
  };

let startSolidPass =
    (~pixelRatio, ~scaleFactor, ~screenWidth, ~screenHeight, ~projection, ()) => {
  _activePass :=
    Some(
      SolidPass(
        DrawContext.create(
          ~pixelRatio,
          ~scaleFactor,
          ~screenWidth,
          ~screenHeight,
          ~projection,
          (),
        ),
      ),
    );
};

let endSolidPass = () => {
  _activePass := None;
};

let startAlphaPass =
    (~pixelRatio, ~scaleFactor, ~screenWidth, ~screenHeight, ~projection, ()) => {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  _activePass :=
    Some(
      AlphaPass(
        DrawContext.create(
          ~pixelRatio,
          ~scaleFactor,
          ~screenWidth,
          ~screenHeight,
          ~projection,
          (),
        ),
      ),
    );
};

let endAlphaPass = () => {
  glDisable(GL_BLEND);
  _activePass := None;
};
