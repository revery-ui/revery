open Revery_UI.React;

let effect:
  (
    Hooks.Effect.condition('a),
    unit => option(unit => unit),
    Hooks.t(Hooks.Effect.t('a) => 'b, 'c)
  ) =>
  (unit, Hooks.t('b, 'c));
