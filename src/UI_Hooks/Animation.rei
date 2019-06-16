open Revery_UI.Animated;
open Revery_UI.React.Hooks;

let animation:
  (
    animationValue,
    animationOptions,
    t(
      (Ref.t(animationValue), Reducer.t(int), Effect.t(Effect.onMount)) =>
      'a,
      unit,
      'b,
      (Ref.t(animationValue), Reducer.t(int), Effect.t(Effect.onMount)) =>
      'c,
    )
  ) =>
  (float, t('a, unit, 'b, 'c));
