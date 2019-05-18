open Revery_UI.React.Hooks;

let state:
  ('a, t(State.t('a) => 'b, unit, 'c, State.t('a) => 'd)) =>
  ('a, 'a => unit, t('b, unit, 'c, 'd));
