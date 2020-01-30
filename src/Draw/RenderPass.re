module DrawContext = {
  type t = {
    canvas: CanvasContext.t,
    screenWidth: int,
    screenHeight: int,
  };

  let create = (~canvas, ~screenWidth, ~screenHeight, ()) => {
    canvas,
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

let start = (~canvas, ~screenWidth, ~screenHeight, ()) => {
  _activeContext :=
    Some(DrawContext.create(~canvas, ~screenWidth, ~screenHeight, ()));
};

let endAlphaPass = () => {
  _activeContext := None;
};
