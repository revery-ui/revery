open Effect;
open Reducer;
open Tick;
open Ref;
open State;

let time = (~tickRate=Time.zero, ()) => {
  let%hook (time, setTime) = reducer(~initialState=Time.now(), t => t);
  let%hook () = tick(~tickRate, _dt => setTime(_t => Time.now()));

  time;
};

// TODO: Workaround for https://github.com/briskml/brisk-reconciler/issues/27
// Remove when fixed
let mountIfEffect = (condition, handler) => {
  let%hook (maybeDispose, _setDispose) = state(Stdlib.ref(None));
  let mountCleanup = () =>
    switch (maybeDispose^) {
    | Some(dispose) =>
      dispose();
      maybeDispose := None;
    | None => ()
    };

  let%hook () =
    effect(
      OnMount,
      () => {
        maybeDispose := handler();
        Some(mountCleanup);
      },
    );

  let%hook () =
    effect(
      condition,
      () => {
        mountCleanup();
        handler();
      },
    );

  ();
};

let timer = (~tickRate=Time.zero, ~active=true, ()) => {
  let%hook (time, setTime) = reducer(~initialState=Time.now(), t => t);
  let%hook (startTime, setStartTime) = Ref.ref(time);

  let onTick = _dt => setTime(_t => Time.now());

  let%hook () =
    mountIfEffect(If((!=), active), () =>
      if (active) {
        setStartTime(Time.(now() - (time - startTime)));
        let dispose = Revery_Core.Tick.interval(onTick, tickRate);
        Some(dispose);
      } else {
        None;
      }
    );

  let reset = () => {
    setStartTime(time);
  };

  (Time.(time - startTime), reset);
};
