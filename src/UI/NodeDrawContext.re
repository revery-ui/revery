/*
 * NodeDrawContext
 *
 * This is context that is passed from parent -> child nodes when redrawing the scene
 */
open Reglm;

type t = {
  transform: Mat4.t,
  zIndex: int,
  opacity: float,
};

let create = (transform: Mat4.t, zIndex: int, opacity: float) => {
  let ret: t = {transform, zIndex, opacity};
  ret;
};

let createFromParent =
    (parentContext: t, localTransform: Mat4.t, localOpacity: float) => {
  let transform = Mat4.create();
  Mat4.multiply(transform, parentContext.transform, localTransform);

  let zIndex = parentContext.zIndex + 1;
  let opacity = parentContext.opacity *. localOpacity;

  let ret: t = {transform, zIndex, opacity};
  ret;
};
