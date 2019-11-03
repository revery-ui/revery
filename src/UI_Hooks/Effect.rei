open Revery_UI.React.Hooks;

let effect:
  (
    Effect.condition('a),
    unit => option(unit => unit),
    t(Effect.t('a) => 'b, 'c)
  ) =>
  (unit, t('b, 'c));
