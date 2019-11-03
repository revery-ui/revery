open Revery_UI.React;

let state:
  ('a, Hooks.t(Hooks.State.t('a) => 'b, 'c)) =>
  (('a, ('a => 'a) => unit), Hooks.t('b, 'c));
