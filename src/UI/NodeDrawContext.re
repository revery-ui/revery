/*
 * NodeDrawContext
 *
 * This is context that is passed from parent -> child nodes when redrawing the scene
 */
type t = {
  zIndex: int,
  opacity: float,
  pixelRatio: float,
  screenHeight: int,
  scaleFactor: int,
};

let create =
    (
      pixelRatio: float,
      zIndex: int,
      opacity: float,
      screenHeight: int,
      scaleFactor: int,
    ) => {
  let ret: t = {zIndex, opacity, pixelRatio, screenHeight, scaleFactor};
  ret;
};

let createFromParent = (parentContext: t, localOpacity: float) => {
  let zIndex = parentContext.zIndex + 1;
  let opacity = parentContext.opacity *. localOpacity;
  let pixelRatio = parentContext.pixelRatio;
  let screenHeight = parentContext.screenHeight;
  let scaleFactor = parentContext.scaleFactor;

  let ret: t = {zIndex, opacity, pixelRatio, screenHeight, scaleFactor};
  ret;
};
