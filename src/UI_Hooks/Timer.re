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
  let%hook startTime = Ref.ref(time);
  let%hook lastDispose = Ref.ref(None);

  let onTick = _dt => setTime(_t => Time.now());

  let%hook () =
    effect(OnMountAndIf((!=), active), () =>
      if (active) {
        startTime := Time.(now() - (time - startTime^));

        switch (lastDispose^) {
        | None => ()
        | Some(f) => f()
        };

        let stopInterval = Revery_Core.Tick.interval(onTick, tickRate);
        lastDispose := Some(stopInterval);

        Some(stopInterval);
      } else {
        switch (lastDispose^) {
        | None => ()
        | Some(f) => f()
        };
        lastDispose := None;

        None;
      }
    );

  let reset = () => {
    startTime := time;
  };

  (Time.(time - startTime^), reset);
};
