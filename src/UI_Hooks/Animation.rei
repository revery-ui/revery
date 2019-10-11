open Revery_UI.Animated;
open Revery_UI.React.Hooks;

let animation:
  (
    animationValue,
    animationOptions,
    t(
      (ref(animationValue), Reducer.t(int), Effect.t(Effect.onMount)) => 'a,
      'b,
    )
  ) =>
  (float, t('a, 'b));
