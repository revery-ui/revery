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
};

let create =
    (pixelRatio: float, zIndex: int, opacity: float, screenHeight: int) => {
  let ret: t = {zIndex, opacity, pixelRatio, screenHeight};
  ret;
};

let createFromParent = (parentContext: t, localOpacity: float) => {
  let zIndex = parentContext.zIndex + 1;
  let opacity = parentContext.opacity *. localOpacity;
  let pixelRatio = parentContext.pixelRatio;
  let screenHeight = parentContext.screenHeight;

  let ret: t = {zIndex, opacity, pixelRatio, screenHeight};
  ret;
};
