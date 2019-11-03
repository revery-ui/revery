open Revery_UI.React.Hooks;

let reducer:
  (
    ~initialState: 'a,
    ('b, 'a) => 'a,
    Revery_UI.React.Hooks.t(Revery_UI.React.Hooks.Reducer.t('a) => 'c, 'd)
  ) =>
  (('a, 'b => unit), Revery_UI.React.Hooks.t('c, 'd));
