let ref:
  (
    'a,
    Revery_UI.React.Hooks.t(
      Revery_UI.React.Hooks.Ref.t('a) => 'b,
      unit,
      'c,
      Revery_UI.React.Hooks.Ref.t('a) => 'd,
    )
  ) =>
  ('a, 'a => unit, Revery_UI.React.Hooks.t('b, unit, 'c, 'd));
