/* Hooks specific to Revery */

let animation =
    (v: Animated.animationValue, opts: Animated.animationOptions, slots) => {
  let (currentV, _set, slots) = UiReact.Hooks.state(v, slots);

  let slots =
    UiReact.Hooks.effect(
      OnMount,
      () => {
        let anim = Animated.start(v, opts);

        Some(() => Animated.cancel(anim));
      },
      slots,
    );

  (currentV.current, slots);
};
