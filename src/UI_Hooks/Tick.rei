open Revery_UI.React.Hooks;

module Time = Revery_Core.Time;

let tick:
  (
    ~tickRate: Time.t=?,
    Time.t => unit,
    t(
      Effect.t(Effect.onMount) => 'b,
      'c,
    )
  ) =>
  t('b, 'c);
