open Revery_UI.React;

module Time = Revery_Core.Time;

let tick:
  (
    ~tickRate: Time.t=?,
    Revery_Core__Tick.callback,
    Hooks.t(Hooks.Effect.t(Hooks.Effect.onMount) => 'a, 'b)
  ) =>
  (unit, Hooks.t('a, 'b));
