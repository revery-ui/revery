module Time = Revery_Core.Time;

module Hooks = Revery_UI.React.Hooks;

let tick = (~tickRate=Time.Seconds(1.), onTick, hooks) =>
  Hooks.effect(
    OnMount,
    () => {
      let dispose = Revery_Core.Tick.interval(onTick, tickRate);

      Some(dispose);
    },
    hooks,
  );
