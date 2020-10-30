module Time = Revery_Core.Time;
module Spring = Revery_UI.Spring;

let spring =
    (
      ~enabled=true,
      ~name,
      ~target,
      ~initialState=?,
      ~restThreshold=0.1,
      options,
    ) => {
  let initialState =
    switch (initialState) {
    | Some(state) => state
    | None => Spring.create(target, Time.now())
    };

  let%hook previousState = Ref.ref(initialState);

  let isActive =
    enabled
    && (
      !Spring.isAtRest(~restThreshold, previousState^)
      || Float.abs(target -. previousState^.value) > restThreshold
    );

  let%hook (time, _) = Timer.timer(~name, ~active=isActive, ());

  let state =
    if (enabled) {
      Spring.tick(target, previousState^, options, time);
    } else {
      Spring.setPosition(target, previousState^);
    };

  previousState := state;

  let setImmediately = position =>
    previousState := Spring.setPosition(position, previousState^);

  let v = isActive ? state.value : target;

  (v, setImmediately);
};
