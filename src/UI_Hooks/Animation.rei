open Revery_UI.Animated;
open Revery_UI.React.Hooks;

let animation:
  (
    animationValue,
    animationOptions,
    t(
      (
        Ref.t(animation),
        Reducer.t(int),
        Effect.t(Effect.onMount)
      ) =>
      'a,
      unit,
      'b,
      (
        Ref.t(animation),
        Reducer.t(int),
        Effect.t(Effect.onMount)
      ) =>
      'c,
    )
  ) =>
  (float, unit => (unit => unit), unit => unit, t('a, unit, 'b, 'c));
