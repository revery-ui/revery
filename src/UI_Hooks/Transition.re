/* Hooks specific to Revery */
open Revery_Core;
open Revery_UI.Animated;

let transition =
    (toValue, ~delay=Time.Seconds(0.0), ~duration=Time.Seconds(1.), slots) => {
  let repeat = false;
  let ({value, _}, pauseAnim, _restartAnim, setAnim, slots) =
    Animation.animation'(
      floatValue(toValue),
      options(~toValue, ~duration, ~delay=Time.Seconds(0.0), ~repeat, ()),
      slots,
    );
  let setAnim = toValue => {
    let animation =
      tween(value, options(~toValue, ~duration, ~delay, ~repeat, ()));
    // only for cleaning purpose we don't restart it
    let _: unit => unit = pauseAnim();
    let newActiveAnim = {
      animation,
      update: None,
      complete: Some(() => animation.value.current = toValue),
    };
    addAnimation(newActiveAnim);
    setAnim(animation);
  };
  (value.current, setAnim, slots);
};
