module Transition: {
  /**
  {2  Description:}

  This is a transition hook

  {2 Usage:}

  {[
  let (transitionedOpacity, setTransitionedOpacity, hooks) =
    Hooks.transition(1., ~duration=Seconds(1.), hooks);

  let onMouseDown  = () => setTransitionedOpacity(0.5);
  let onMouseUp = () => setTransitionedOpacity(1.0);

  <Opacity opacity=transitionedOpacity>
    ...
  </Opacity>
  ]}
  */
  let transition:
    (
      float,
      ~delay: Revery_Core.Time.t=?,
      ~duration: Revery_Core.Time.t=?,
      Revery_UI.React.Hooks.t(
        (
          ref(Revery_UI.Animated.animation),
          Revery_UI.React.Hooks.Reducer.t(int),
          Revery_UI.React.Hooks.Effect.t(Revery_UI.React.Hooks.Effect.onMount)
        ) =>
        'a,
        'b,
      )
    ) =>
    (
      (float, (~immediate: bool=?, float) => unit),
      Revery_UI.React.Hooks.t('a, 'b),
    );
};

let animation:
  (
    Revery_UI.Animated.animationValue,
    Revery_UI.Animated.animationOptions,
    Revery_UI.React.Hooks.t(
      (
        ref(Revery_UI.Animated.animation),
        Revery_UI.React.Hooks.Reducer.t(int),
        Revery_UI.React.Hooks.Effect.t(Revery_UI.React.Hooks.Effect.onMount)
      ) =>
      'a,
      'b,
    )
  ) =>
  (
    (float, (unit, unit) => unit, unit => unit),
    Revery_UI.React.Hooks.t('a, 'b),
  );
