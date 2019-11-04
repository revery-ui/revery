/* Hooks specific to Revery */
module TickHook = Tick;
open Revery_Core;
module Animation = Revery_UI.Animated;


let animation' = (value: float, opts: Animation.animationOptions, slots) => {
  let (animation, setAnim, slots) =
    Ref.ref(Animation.tween(value, opts), slots);
  let (_, dispatch, slots) =
    Reducer.reducer(~initialState=0, _ => succ, slots);
  let slots = TickHook.tick(~tickRate=Seconds(0.), _ => dispatch(), slots);

  (
    Animation.getValue(animation),
    () => Animation.pause(animation),
    () => Animation.restart(animation),
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
        Animation.options(~toValue, ~duration, ~delay=Time.Seconds(0.0), ~repeat=false, ()),
        slots,
      );
    let setTarget = (~immediate=false, toValue) => {
      setAnim(
        Animation.tween(
          immediate ? toValue : value,
          Animation.options(~toValue, ~duration, ~delay, ~repeat=false, ()),
        )
      );
    };
    (value, setTarget, slots);
  };
};
