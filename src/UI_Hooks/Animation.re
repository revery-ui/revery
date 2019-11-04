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

let animation' = (value: float, opts: animationOptions) => {
  let%hook (animation, setAnim) = Ref.ref(tween(value, opts));
  let%hook (_, dispatch) = Reducer.reducer(~initialState=0, _ => succ);
  let completer = Tick.interval(_t => dispatch(), Time.zero);

  let%hook () = Effect.effect(OnMount, animationLoop(animation, completer));

  (
    getValue(animation),
    () => pause(~completer, animation),
    () => restart(~completer, animation),
    setAnim,
  );
};

let animation = (v, opts) => {
  let%hook (value, pause, restart, _setAnim) = animation'(v, opts);
  (value, pause, restart);
};

module Transition = {
  let transition = (toValue, ~delay=Time.zero, ~duration=Time.seconds(1.)) => {
    let%hook (value, pauseAnim, _restartAnim, setAnim) =
      animation'(
        toValue,
        options(~toValue, ~duration, ~delay=Time.zero, ~repeat=false, ()),
      );
    let setAnim = (~immediate=false, toValue) => {
      let animation =
        tween(
          immediate ? toValue : value,
          options(~toValue, ~duration, ~delay, ~repeat=false, ()),
        );

      // only for cleaning purpose we don't restart it
      let _resume: unit => unit = pauseAnim();

      let _: playback =
        start(animation, ~complete=() => setValue(toValue, animation));
      setAnim(animation);
    };
    (value, setAnim);
  };
};