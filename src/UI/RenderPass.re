open Reglm;

type renderPass =
  | SolidPass(Mat4.t)
  | AlphaPass(Mat4.t);

type t = renderPass;
