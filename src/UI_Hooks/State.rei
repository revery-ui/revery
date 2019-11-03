open Revery_UI.React.Hooks;

let state:
  (
    'a,
    Revery_UI.React.Hooks.t(Revery_UI.React.Hooks.State.t('a) => 'b, 'c)
  ) =>
  (('a, ('a => 'a) => unit), Revery_UI.React.Hooks.t('b, 'c));
