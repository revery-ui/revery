open Revery_UI.React;

let reducer:
  (
    ~initialState: 'a,
    ('b, 'a) => 'a,
    Hooks.t(Hooks.Reducer.t('a) => 'c, 'd)
  ) =>
  (('a, 'b => unit), Hooks.t('c, 'd));
