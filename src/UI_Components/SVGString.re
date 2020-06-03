open Revery_UI_Primitives;

let make = (~style, ~contents, ()) => {
  let render = Revery_SVG.render(~width=100., ~height=100., contents);

  <Canvas style render />;
};
