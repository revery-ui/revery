open Effect;
open Reducer;
open Tick;

let time = (~tickRate=Time.zero, ()) => {
  let%hook (time, setTime) = reducer(~initialState=Time.now(), t => t);
  let%hook () = tick(~tickRate, _dt => setTime(_t => Time.now()));

  time;
};

let timer = (~tickRate=Time.zero, ~active=true, ()) => {
  let%hook (time, setTime) = reducer(~initialState=Time.now(), t => t);
  let%hook (startTime) = Ref.ref(time);

  let onTick = _dt => setTime(_t => Time.now());

  let%hook () = effect(OnMountAndIf((!=), active), () =>
      if (active) {
        startTime := (Time.(now() - (time - startTime^)));
        let dispose = Revery_Core.Tick.interval(onTick, tickRate);
        Some(dispose);
      } else {
        None;
      }
    );

  let reset = () => {
    startTime := time;
  };

  (Time.(time - startTime^), reset);
};
