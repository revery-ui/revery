open Revery_UI.React.Hooks;

module Time = Revery_Core.Time;

let tick:
  (
    ~tickRate: Time.t=?,
    Time.t => unit,
    t(
      Effect.t(Effect.onMount) => 'b,
      unit,
      'c,
      Effect.t(Effect.onMount) => 'd,
    )
  ) =>
  t('b, unit, 'c, 'd);
