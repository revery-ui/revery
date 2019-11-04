/* Hooks specific to Revery */
module TickHook = Tick;
open Revery_Core;
module Animation = Revery_UI.Animated;


let animation' = (value: float, opts: Animation.animationOptions) => {
  let%hook (animation, setAnim) = Ref.ref(Animation.tween(value, opts));
  let%hook (_, dispatch) = Reducer.reducer(~initialState=0, _ => succ);
  let%hook () = TickHook.tick(~tickRate=Time.zero, _ => dispatch());

  (
    Animation.getValue(animation),
    () => Animation.pause(animation),
    () => Animation.restart(animation),
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
        Animation.options(~toValue, ~duration, ~delay=Time.zero, ~repeat=false, ()),
      );
    let setTarget = (~immediate=false, toValue) => {
      setAnim(
        Animation.tween(
          immediate ? toValue : value,
          Animation.options(~toValue, ~duration, ~delay, ~repeat=false, ()),
        )
      );
    };
    (value, setTarget);
  };
};