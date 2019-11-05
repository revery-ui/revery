include Animation;
include Effect;
include Reducer;
include Ref;
include State;
include Tick;

let time = (~tickRate=Time.Seconds(0.), hooks) => {
  let (time, setTime, hooks) =
    reducer(~initialState=Time.getTime(), t => t, hooks);
  let hooks = tick(~tickRate, _dt => setTime(_t => Time.getTime()), hooks);

  (time, hooks);
};

// TODO: Workaround for https://github.com/briskml/brisk-reconciler/issues/27
// Remove when fixed
let mountIfEffect = (condition, handler, hooks) => {
  let (maybeDispose, setDispose, hooks) = Ref.ref(None, hooks);
  let mountCleanup = () =>
    switch (maybeDispose) {
    | Some(dispose) =>
      dispose();
      setDispose(None);
    | None => ()
    };

  let hooks =
    effect(
      OnMount,
      () => {
        setDispose(handler());
        Some(mountCleanup);
      },
      hooks,
    );

  let hooks =
    effect(
      condition,
      () => {
        mountCleanup();
        handler();
      },
      hooks,
    );

  hooks;
};

let timer = (~tickRate=Time.Seconds(0.), ~active=true, hooks) => {
  let (time, setTime, hooks) =
    reducer(~initialState=Time.getTime(), t => t, hooks);
  let (startTime, setStartTime, hooks) = state(time, hooks);

  let onTick = _dt => setTime(_t => Time.getTime());

  let hooks =
    mountIfEffect(If((!=), active), () =>
      if (active) {
        let dispose = Revery_Core.Tick.interval(onTick, tickRate);
        Some(dispose);
      } else {
        None;
      },
      hooks
    );

  let reset = () => {
    setStartTime(time);
  };

  (
    Time.ofSeconds(Time.toSeconds(time) -. Time.toSeconds(startTime)),
    reset,
    hooks,
  );
};
