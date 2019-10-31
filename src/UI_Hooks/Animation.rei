open Revery_UI.Animated;
open Revery_UI.React.Hooks;

let animation':
  (
    animationValue,
    animationOptions,
    t((ref(animation), Reducer.t(int), Effect.t(Effect.onMount)) => 'a, 'b)
  ) =>
  (
    animation,
    (unit, unit) => unit,
    unit => unit,
    animation => unit,
    t('a, 'b),
  );
let animation:
  (
    animationValue,
    animationOptions,
    t((ref(animation), Reducer.t(int), Effect.t(Effect.onMount)) => 'a, 'b)
  ) =>
  (float, (unit, unit) => unit, unit => unit, t('a, 'b));
