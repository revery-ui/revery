/* Hooks specific to Revery */
open Animated;

let animation = (v: animationValue, opts: animationOptions, slots) => {
  let (currentV, _set, slots) = UiReact.Hooks.state(v, slots);

  let slots =
    UiReact.Hooks.effect(
      OnMount,
      () => {
        let {stop, _} = tween(v, opts) |> start;

        Some(() => stop());
      },
      slots,
    );

  (currentV.current, slots);
};