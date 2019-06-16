open Revery_UI.React.Hooks;

let effect:
  (
    Effect.condition('a),
    Effect.handler,
    t(Effect.t('a) => 'b, unit, 'c, Effect.t('a) => 'd)
  ) =>
  t('b, unit, 'c, 'd);
