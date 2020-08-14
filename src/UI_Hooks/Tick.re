module Time = Revery_Core.Time;
module Tick = Revery_Core.Tick;

module Hooks = Revery_UI.React.Hooks;

let tick = (~tickRate=Time.seconds(1), onTick) => {
  // Because Tick.interval is only called once, initiallly, with the initial
  // onTick function, to execute the latest onTick function we either have to
  // dispose and recreate it for every call, or use a mutable variable to replace
  // the callback. Here we do the latter.
  let%hook onTickRef = Hooks.ref(onTick);
  onTickRef := onTick;

  let%hook () =
    Hooks.effect(
      OnMount,
      () => {
        let dispose = Tick.interval(t => onTickRef^(t), tickRate);

        Some(dispose);
      },
    );

  ();
};
