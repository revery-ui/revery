open Revery_UI.React;

let ref:
  ('a, Hooks.t(ref('a) => 'b, 'c)) => (('a, 'a => unit), Hooks.t('b, 'c));
