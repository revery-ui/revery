open Revery;
open Revery.UI;
open Revery.UI.Components;

module FPSCounter = {
  type fps = float;

  let%component make = (~w, ()) => {
    let updateFPS = ((),s) => {
      Window.getFPS(w);
    };

    let%hook (fps, dispatch) = Hooks.reducer(
      ~initialState=0.0,
      updateFPS,
    );

    let showFPS = () => Printf.sprintf("FPS: %.0f",fps);

    let viewStyle =
      Style.[
        position(`Absolute),
        left(0),
        right(0),
        top(0),
        bottom(0),
        flexDirection(`Column),
      ];

    <View style=viewStyle>
      <Text text=showFPS() />
    </View>;
  };
};

let render = w => <FPSCounter w />;
