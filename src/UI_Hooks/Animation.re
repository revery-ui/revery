/* Hooks specific to Revery */
open Revery_Core;
open Revery_UI.Animated;

let reducer = (_a, s) => s + 1;

let animationLoop = (dispatch, v, opts, ()) => {
  let complete = Tick.interval(_t => dispatch(), Seconds(0.));
  let {stop, _} = tween(v, opts) |> start(~complete);

  Some(
    () => {
      Log.info("Hooks - Animation", "Stopping animation");
      stop();
      complete();
    },
  );
};

let animation = (v: animationValue, opts: animationOptions) => {
  let%hook (currentV, _) = Ref.ref(v);
  let%hook (_, dispatch) = Reducer.reducer(~initialState=0, reducer);

  let%hook () =
    Effect.effect(OnMount, animationLoop(dispatch, currentV, opts));

  currentV.current;
};
