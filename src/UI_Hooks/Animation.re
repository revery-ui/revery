/* Hooks specific to Revery */
open Revery_Core;
open Revery_UI.Animated;
open Revery_UI.Easing;

let reducer = (_a, s) => s + 1;

let animationLoop = (animation, completer, ()) => {
  let complete = completer();
  let {stop, _} = animation |> start(~complete);
  Some(
    () => {
      Log.info("Hooks - Animation", "Stopping animation");
      stop();
      complete();
    },
  );
};

module Transition = {
  let animation' = (v: animationValue, opts: animationOptions, slots) => {
    let (animation, setAnim, slots) = Ref.ref(tween(v, opts), slots);
    let (_, dispatch, slots) =
      Reducer.reducer(~initialState=0, reducer, slots);
    let completer = () => Tick.interval(_t => dispatch(), Seconds(0.));

    let restart = () => {
      animation.startTime = Time.to_float_seconds(getTime());
      animation.value.current = animation.startValue;
      let newActiveAnim = {
        animation,
        update: None,
        complete: Some(completer()),
      };
      addAnimation(newActiveAnim);
    };

    let pause = () => {
      removeAnimation(animation);
      () => {
        let newActiveAnim = {
          animation,
          update: None,
          complete: Some(completer()),
        };
        addAnimation(newActiveAnim);
      };
    };

    let slots =
      Effect.effect(OnMount, animationLoop(animation, completer), slots);

    (animation, pause, restart, setAnim, slots);
  };

  let transition =
      (
        ~duration=Time.Seconds(1.),
        ~delay=Time.Seconds(0.0),
        toValue,
        ~easing=linear,
        slots,
      ) => {
    let repeat = false;
    let ({value, _}, pauseAnim, _restartAnim, setAnim, slots) =
      animation'(
        floatValue(toValue),
        options(~toValue, ~duration, ~delay=Time.Seconds(0.0), ~repeat, ()),
        slots,
      );
    let setAnim = (~immediate=false, toValue) => {
      let animation =
        tween(
          immediate ? floatValue(toValue) : value,
          options(~duration, ~delay, ~toValue, ~easing, ~repeat, ()),
        );
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
};

let animation = (v, opts, slots) => {
  let (animation, pause, restart, _setAnim, slots) =
    Transition.animation'(v, opts, slots);
  (animation.value.current, pause, restart, slots);
};
