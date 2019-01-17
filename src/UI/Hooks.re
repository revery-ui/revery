/* Hooks specific to Revery */

/* module React = UiReact; */

let animation =
    (v: Animated.animationValue, opts: Animated.animationOptions, slots) => {
    let (_currentV, _set, slots) = UiReact.Hooks.state(v, slots);

    let slots = UiReact.Hooks.effect(OnMount, () => {
        prerr_endline ("hooked");
        let anim = Animated.start(v, opts);

        Some(() => Animated.cancel(anim));
    }, slots);

  /* TODO: Replace this once 'effect' is working */
  (opts.toValue, slots)
};
