/*
 * NodeDrawContext
 *
 * This is context that is passed from parent -> child nodes when redrawing the scene
 */
type t = {
  zIndex: int,
  dpi: float,
  canvasScalingFactor: float,
  opacity: float,
  canvas: Revery_Draw.CanvasContext.t,
  debug: bool,
};

let create =
    (
      ~dpi,
      ~canvasScalingFactor,
      ~debug,
      ~canvas,
      ~zIndex: int,
      ~opacity: float,
      (),
    ) => {
  dpi,
  canvasScalingFactor,
  debug,
  canvas,
  zIndex,
  opacity,
};

let createFromParent = (parentContext: t, localOpacity: float) => {
  let zIndex = parentContext.zIndex + 1;
  let opacity = parentContext.opacity *. localOpacity;

  {...parentContext, zIndex, opacity};
};
