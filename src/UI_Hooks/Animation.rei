open Revery_UI.Animated;
open Revery_UI.React.Hooks;

let animation:
  (
    animationValue,
    animationOptions,
    t(
      (
        Ref.t(animation),
        Ref.t(animationController),
        Reducer.t(int),
        Effect.t(Effect.onMount)
      ) =>
      'a,
      unit,
      'b,
      (
        Ref.t(animation),
        Ref.t(animationController),
        Reducer.t(int),
        Effect.t(Effect.onMount)
      ) =>
      'c,
    )
  ) =>
  (float, animationController, t('a, unit, 'b, 'c));
