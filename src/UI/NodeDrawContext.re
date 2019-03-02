/*
 * NodeDrawContext
 *
 * This is context that is passed from parent -> child nodes when redrawing the scene
 */
type t = {
  zIndex: int,
  opacity: float,
};

let create = (~zIndex: int, ~opacity: float, ()) => {zIndex, opacity};

let createFromParent = (parentContext: t, localOpacity: float) => {
  let zIndex = parentContext.zIndex + 1;
  let opacity = parentContext.opacity *. localOpacity;

  {zIndex, opacity};
};
