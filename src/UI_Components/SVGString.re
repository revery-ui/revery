open Revery_UI;
open Revery_UI_Primitives;

let make = (~style, ~contents, ()) => {
  let render = (canvas, Dimensions.{width, height, _}) =>
    Revery_SVG.render(~width, ~height, contents, canvas);

  <Canvas style render />;
};
