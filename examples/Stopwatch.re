open Revery;
open Revery.Time;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

module Clock = {
  type dispose = unit => unit;
  let noop = () => ();

  type state = {
    dispose,
    isRunning: bool,
    elapsedTime: Time.t,
  };

  type action =
    | Start(dispose)
    | Stop
    | TimerTick(Time.t);

  let reducer = (a, s) =>
    switch (a) {
    | Start(f) => {dispose: f, isRunning: true, elapsedTime: Seconds(0.)}
    | Stop =>
      s.dispose();
      let ret = {dispose: noop, isRunning: false, elapsedTime: Seconds(0.)};
      ret;
    | TimerTick(t) => {
        ...s,
        elapsedTime:
          s.isRunning ? Time.increment(s.elapsedTime, t) : s.elapsedTime,
      }
    };

  let component = React.component("Clock");

  let createElement = (~children as _, ()) =>
    component(hooks => {
      let (state, dispatch, hooks) =
        Hooks.reducer(
          ~initialState={
            isRunning: false,
            dispose: noop,
            elapsedTime: Seconds(0.),
          },
          reducer,
          hooks,
        );

      /*
       * We'll make sure to dispatch the 'Stop' action when unmounting,
       * so we don't have a runaway timer!
       */
      let hooks =
        Hooks.effect(OnMount, () => Some(() => dispatch(Stop)), hooks);

      let startStop = () =>
        state.isRunning
          ? dispatch(Stop)
          /*
           * If we're not already running, we'll start a timer job
           * and use the delta time it passes to update our reducer.
           */
          : {
            let dispose =
              Tick.interval(t => dispatch(TimerTick(t)), Seconds(0.));

            /* We'll also keep a handle on the dispose function so we can make sure its called on stop*/
            dispatch(Start(dispose));
          };

      let buttonText = state.isRunning ? "STOP" : "START";

      let marcherOpacity = state.isRunning ? 1.0 : 0.0;
      let getMarcherPosition = t =>
        sin(Time.to_float_seconds(t) *. 2. *. pi) /. 2. +. 0.5;

      (
        hooks,
        <View
          style=Style.[
            position(`Absolute),
            justifyContent(`Center),
            alignItems(`Center),
            bottom(0),
            top(0),
            left(0),
            right(0),
          ]>
          <View
            style=Style.[
              margin(20),
              width(150),
              borderBottom(~color=Colors.gray, ~width=2),
            ]>
            <Text
              style=Style.[
                color(Colors.white),
                fontFamily("Roboto-Regular.ttf"),
                fontSize(24),
                marginVertical(20),
                width(200),
              ]
              text={string_of_float(
                state.elapsedTime |> Time.to_float_seconds,
              )}
            />
            <View
              style=Style.[
                position(`Absolute),
                bottom(0),
                opacity(marcherOpacity),
                left(
                  int_of_float(
                    getMarcherPosition(state.elapsedTime) *. 146.,
                  ),
                ),
                width(4),
                height(4),
                backgroundColor(Color.hex("#90f7ff")),
              ]
            />
          </View>
          <Button title=buttonText onClick=startStop />
        </View>,
      );
    });
};

let render = () => <Clock />;
