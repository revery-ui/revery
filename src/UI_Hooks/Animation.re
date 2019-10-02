/* Hooks specific to Revery */
open Revery_Core;
open Revery_UI.Animated;

let reducer = (_a, s) => s + 1;

let animationLoop = (dispatch, animation, controller, ()) => {
  // Creates an interval timer that continually executes (every 0 seconds)
  let complete = Tick.interval(_t => dispatch(), Seconds(0.));
  // Starts the animation
  let {pause, stop} = animation |> start(~complete);
  controller#updateLastActive();
  Some(
    () => {
      stop();
      complete();
    },
  );
};

let animation = (v: animationValue, opts: animationOptions, slots) => {
  let (animation, _, slots) = Ref.ref(tween(v, opts), slots);
  let (controller, _, slots) = Ref.ref(new animationController(animation), slots);
  let (_, dispatch, slots) =
    Reducer.reducer(~initialState=0, reducer, slots);

  let slots =
    Effect.effect(OnMount, animationLoop(dispatch, animation, controller), slots);


  (animation.value.current, controller, slots);
};
