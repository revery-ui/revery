/* Hooks specific to Revery */
open Revery_Core;
open Revery_UI.Animated;

let animationLoop = (animation, complete, ()) => {
  let {stop, _} = animation |> start(~complete);
  Some(
    () => {
      Log.info("Hooks - Animation", "Stopping animation");
      stop();
      complete();
    },
  );
};

let animation' = (value: float, opts: animationOptions, slots) => {
  let (animation, setAnim, slots) = Ref.ref(tween(value, opts), slots);
  let (_, dispatch, slots) =
    Reducer.reducer(~initialState=0, _ => succ, slots);
  let completer = Tick.interval(_t => dispatch(), Seconds(0.));

  let slots =
    Effect.effect(OnMount, animationLoop(animation, completer), slots);

  (
    getValue(animation),
    () => pause(~completer, animation),
    () => restart(~completer, animation),
    setAnim,
    slots,
  );
};

let animation = (v, opts, slots) => {
  let (value, pause, restart, _setAnim, slots) =
    animation'(v, opts, slots);
  (value, pause, restart, slots);
};

module Transition = {
  let transition =
      (toValue, ~delay=Time.Seconds(0.0), ~duration=Time.Seconds(1.), slots) => {
    let (value, pauseAnim, _restartAnim, setAnim, slots) =
      animation'(
        toValue,
        options(~toValue, ~duration, ~delay=Time.Seconds(0.0), ~repeat=false, ()),
        slots,
      );
    let setAnim = (~immediate=false, toValue) => {
      let animation =
        tween(
          immediate ? toValue : value,
          options(~toValue, ~duration, ~delay, ~repeat=false, ()),
        );

      // only for cleaning purpose we don't restart it
      let _resume: unit => unit = pauseAnim();

      let _: playback = start(animation);
      setAnim(animation);
    };
    (value, setAnim, slots);
  };
};
