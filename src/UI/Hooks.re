/* Hooks specific to Revery */

/* module React = UiReact; */

let animation =
    (v: Animated.animationValue, opts: Animated.animationOptions, slots) => {
    let (currentV, _set, slots) = UiReact.Hooks.state(v, slots);

    let slots = UiReact.Hooks.effect(OnMount, () => {
        prerr_endline ("Starting animation");
        let anim = Animated.start(v, opts);

        Some(() => {
            prerr_endline ("Stopping animation");
            Animated.cancel(anim);
        });
    }, slots);

  /* TODO: Replace this once 'effect' is working */
  (currentV.current, slots)
};
