let effect:
  (
    Revery_UI.React.Hooks.Effect.condition('a),
    Revery_UI.React.Hooks.Effect.handler,
    Revery_UI.React.Hooks.t(
      Revery_UI.React.Hooks.Effect.t('a) => 'b,
      unit,
      'c,
      Revery_UI.React.Hooks.Effect.t('a) => 'd,
    )
  ) =>
  Revery_UI.React.Hooks.t('b, unit, 'c, 'd);
