open Revery_UI.React.Hooks;

let ref:
  ('a, t(Ref.t('a) => 'b, unit, 'c, Ref.t('a) => 'd)) =>
  ('a, 'a => unit, t('b, unit, 'c, 'd));
