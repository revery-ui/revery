open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

module FPSCounter = {

  type dispose = unit => unit;
  let noop = () => ();

  type state = {
    dispose,
    starting: bool,
    running: bool,
    fps: float,
  };
  type action =
    | Start(dispose)
    | Stop
    | FrameTick(float)

  let reducer = (a,s) => switch (a) {
      | Start(d) => {dispose: d, starting: false, running: true, fps: 0.0}
      | Stop =>
        Console.log("---STOP---");
        s.dispose();
        {dispose: noop, starting: false, running: false, fps: 0.0};
      | FrameTick(f) => s.running ? { ...s, starting: false, fps: f } : s
  };

  let%component make = (~w, ()) => {
    let%hook (state, dispatch) = Hooks.reducer(
      ~initialState={dispose: noop, starting: true, running: true, fps: 0.0},
      reducer,
    );
    let%hook () = Hooks.effect(OnMount, () => Some(() => dispatch(Stop)));

    let _ = state.starting && state.running
      ? dispatch(Start(
        Tick.interval(t => {Console.log("tick"); dispatch(FrameTick(Window.getFPS(w)))}, Time.seconds(1))
      ))
      : noop();

    let showFPS = () => Printf.sprintf("FPS: %.2f",state.fps);

    let viewStyle =
      Style.[
        position(`Absolute),
        left(0),
        right(0),
        top(0),
        bottom(0),
        flexDirection(`Column),
      ];

    let helloView = Hello.render();

    <View style=viewStyle>
      <Text text=showFPS() />
      helloView
    </View>;
  };
};

let render = w => <FPSCounter w />;
