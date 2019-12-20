module Time = Revery_Core.Time;

module Hooks = Revery_UI.React.Hooks;

let tick = (~tickRate=Time.seconds(1), onTick) => {
  let%hook () =
    Hooks.effect(
      OnMount,
      () => {
        let dispose = Revery_Core.Tick.interval(onTick, tickRate);

        Some(dispose);
      },
    );
  ();
};
