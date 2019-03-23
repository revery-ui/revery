/* Hooks specific to Revery */
open Animated;

let animation = (v: animationValue, opts: animationOptions, slots) => {
  let (currentV, _set, slots) = React.Hooks.state(v, slots);

  let slots =
    React.Hooks.effect(
      OnMount,
      () => {
        let {stop, _} = tween(v, opts) |> start;

        Some(() => stop());
      },
      slots,
    );

  (currentV.current, slots);
};
