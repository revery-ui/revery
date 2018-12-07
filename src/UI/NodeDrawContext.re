/*
 * NodeDrawContext
 *
 * This is context that is passed from parent -> child nodes when redrawing the scene
 */
type t = {
  zIndex: int,
  opacity: float,
  pixelRatio: int,
};

let create = (pixelRatio: int, zIndex: int, opacity: float) => {
  let ret: t = {zIndex, opacity, pixelRatio};
  ret;
};

let createFromParent = (parentContext: t, localOpacity: float) => {
  let zIndex = parentContext.zIndex + 1;
  let opacity = parentContext.opacity *. localOpacity;
  let pixelRatio = parentContext.pixelRatio;

  let ret: t = {zIndex, opacity, pixelRatio};
  ret;
};
