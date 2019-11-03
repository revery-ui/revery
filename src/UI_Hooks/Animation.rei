open Revery_UI.React;

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
      Hooks.t(
        (
          ref(Revery_UI.Animated.animation),
          Hooks.Reducer.t(int),
          Hooks.Effect.t(Hooks.Effect.onMount)
        ) =>
        'a,
        'b,
      )
    ) =>
    ((float, (~immediate: bool=?, float) => unit), Hooks.t('a, 'b));
};

let animation:
  (
    Revery_UI.Animated.animationValue,
    Revery_UI.Animated.animationOptions,
    Hooks.t(
      (
        ref(Revery_UI.Animated.animation),
        Hooks.Reducer.t(int),
        Hooks.Effect.t(Hooks.Effect.onMount)
      ) =>
      'a,
      'b,
    )
  ) =>
  ((float, (unit, unit) => unit, unit => unit), Hooks.t('a, 'b));
