open Revery_UI.Animated;
open Revery_UI.React.Hooks;

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
      t(
        (
          ref(animation),
          Reducer.t(int),
          Effect.t(Effect.onMount)
        ) =>
        'a,
        'b,
      )
    ) =>
    ((float, (~immediate: bool=?, float) => unit), t('a, 'b));
};

let animation:
  (
    animationValue,
    animationOptions,
    t(
      (
        ref(animation),
        Reducer.t(int),
        Effect.t(Effect.onMount)
      ) =>
      'a,
      'b,
    )
  ) =>
  ((float, (unit, unit) => unit, unit => unit), t('a, 'b));
