open Revery_UI.React.Hooks;

module Time = Revery_Core.Time;

let tick:
  (
    ~tickRate: Time.t=?,
    Revery_Core.Tick.callback,
    t(Effect.t(Effect.onMount) => 'a, 'b)
  ) =>
  (unit, t('a, 'b));
