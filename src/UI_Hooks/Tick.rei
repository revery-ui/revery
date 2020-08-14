open Revery_UI.React.Hooks;

module Time = Revery_Core.Time;

let tick:
  (
    ~tickRate: Time.t=?,
    Time.t => unit,
    t((ref(Time.t => unit), Effect.t(Effect.onMount)) => 'a, 'b)
  ) =>
  (unit, t('a, 'b));
