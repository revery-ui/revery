open Revery_Core;
open Revery_UI;
open Revery_UI_Primitives;

module Log = (val Log.withNamespace("Revery.UI.Components.SVG"));

let make = (~style=?, ~contents, ()) => {
  let render = (canvas, Dimensions.{width, height, _}) =>
    try(Revery_SVG.render(~width, ~height, contents, canvas)) {
    | Failure(error) => Log.error("Render error: " ++ error)
    };

  <Canvas ?style render />;
};
