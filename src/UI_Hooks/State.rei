open Revery_UI.React.Hooks;

let state:
  ('a, t(State.t('a) => 'b, 'c)) => (('a, ('a => 'a) => unit), t('b, 'c));
