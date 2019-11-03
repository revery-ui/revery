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
  let animation' = (v: animationValue, opts: animationOptions) => {
    let%hook (animation, setAnim) = Ref.ref(tween(v, opts));
    let%hook (_, dispatch) = Reducer.reducer(~initialState=0, reducer);
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

    let%hook () =
      Effect.effect(OnMount, animationLoop(animation, completer));

    (animation, pause, restart, setAnim);
  };

  let transition =
      (toValue, ~delay=Time.Seconds(0.0), ~duration=Time.Seconds(1.)) => {
    let repeat = false;
    let%hook ({value, _}, pauseAnim, _restartAnim, setAnim) =
      animation'(
        floatValue(toValue),
        options(~toValue, ~duration, ~delay=Time.Seconds(0.0), ~repeat, ()),
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
    (value.current, setAnim);
  };
};

let animation = (v, opts) => {
  let%hook (animation, pause, restart, _setAnim) =
    Transition.animation'(v, opts);
  (animation.value.current, pause, restart);
};
