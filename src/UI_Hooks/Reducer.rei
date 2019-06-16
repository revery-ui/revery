open Revery_UI.React.Hooks;

let reducer:
  (
    ~initialState: 'a,
    ('b, 'a) => 'a,
    t(Reducer.t('a) => 'c, unit, 'd, Reducer.t('a) => 'e)
  ) =>
  ('a, 'b => unit, t('c, unit, 'd, 'e));
