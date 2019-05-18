let state:
  (
    'a,
    Revery_UI.React.Hooks.t(
      Revery_UI.React.Hooks.State.t('a) => 'b,
      unit,
      'c,
      Revery_UI.React.Hooks.State.t('a) => 'd,
    )
  ) =>
  ('a, 'a => unit, Revery_UI.React.Hooks.t('b, unit, 'c, 'd));
