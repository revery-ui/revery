let effect:
  (
    Revery_UI.React.Hooks.Effect.condition('a),
    unit => option(unit => unit),
    Revery_UI.React.Hooks.t(Revery_UI.React.Hooks.Effect.t('a) => 'b, 'c)
  ) =>
  (unit, Revery_UI.React.Hooks.t('b, 'c));
