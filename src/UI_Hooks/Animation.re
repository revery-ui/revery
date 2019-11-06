/* Hooks specific to Revery */
open Revery_Core;
open Revery_UI.Animated;

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
    let completer = () => Tick.interval(_t => dispatch(), Time.zero);

    let restart = () => {
      animation.startTime = Time.toSeconds(Time.now());
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
      (toValue, ~delay=Time.seconds(0.0), ~duration=Time.seconds(1.), slots) => {
    let repeat = false;
    let ({value, _}, pauseAnim, _restartAnim, setAnim, slots) =
      animation'(
        floatValue(toValue),
        options(~toValue, ~duration, ~delay=Time.seconds(0.0), ~repeat, ()),
        slots,
      );
    let setAnim = (~immediate=false, toValue) => {
      let animation =
        tween(
          immediate ? floatValue(toValue) : value,
          options(~toValue, ~duration, ~delay, ~repeat, ()),
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
