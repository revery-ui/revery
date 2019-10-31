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
    activeAnimations := [newActiveAnim, ...activeAnimations^];
  };

  let pause = () => {
    activeAnimations :=
      List.filter(
        ({animation: a, _}) => a.id !== animation.id,
        activeAnimations^,
      );
    () => {
      let newActiveAnim = {
        animation,
        update: None,
        complete: Some(completer()),
      };
      activeAnimations := [newActiveAnim, ...activeAnimations^];
    };
  };

  let slots =
    Effect.effect(OnMount, animationLoop(animation, completer), slots);

  (animation, pause, restart, setAnim, slots);
};
let animation = (v, opts, slots) => {
  let (animation, pause, restart, _setAnim, slots) =
    animation'(v, opts, slots);
  (animation.value.current, pause, restart, slots);
};
