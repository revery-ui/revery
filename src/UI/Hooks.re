/* Hooks specific to Revery */
open Revery_Core;
open Animated;

let reducer = (_a, s) => s + 1;

let animationLoop = (dispatch, v, opts, ()) => {
  let complete = Tick.interval(_t => dispatch(), Seconds(0.));
  let {stop, _} = tween(v, opts) |> start(~complete);
  Some(
    () => {
      stop();
      complete();
    },
  );
};

let animation = (v: animationValue, opts: animationOptions, slots) => {
  let (currentV, _, slots) = React.Hooks.ref(v, slots);
  let (_, dispatch, slots) =
    React.Hooks.reducer(~initialState=0, reducer, slots);

  let slots =
    React.Hooks.effect(
      OnMount,
      animationLoop(dispatch, currentV, opts),
      slots,
    );

  (currentV.current, slots);
};
