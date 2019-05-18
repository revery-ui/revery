let reducer:
  (
    ~initialState: 'a,
    ('b, 'a) => 'a,
    Revery_UI.React.Hooks.t(
      Revery_UI.React.Hooks.Reducer.t('a) => 'c,
      unit,
      'd,
      Revery_UI.React.Hooks.Reducer.t('a) => 'e,
    )
  ) =>
  ('a, 'b => unit, Revery_UI.React.Hooks.t('c, unit, 'd, 'e));
