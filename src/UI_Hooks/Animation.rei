let animation:
  (
    Revery_UI.Animated.animationValue,
    Revery_UI.Animated.animationOptions,
    Revery_UI.React.Hooks.t(
      (
        Revery_UI.React.Hooks.Ref.t(Revery_UI.Animated.animationValue),
        Revery_UI.React.Hooks.Reducer.t(int),
        Revery_UI.React.Hooks.Effect.t(Revery_UI.React.Hooks.Effect.onMount)
      ) =>
      'a,
      unit,
      'b,
      (
        Revery_UI.React.Hooks.Ref.t(Revery_UI.Animated.animationValue),
        Revery_UI.React.Hooks.Reducer.t(int),
        Revery_UI.React.Hooks.Effect.t(Revery_UI.React.Hooks.Effect.onMount)
      ) =>
      'c,
    )
  ) =>
  (float, Revery_UI.React.Hooks.t('a, unit, 'b, 'c));
