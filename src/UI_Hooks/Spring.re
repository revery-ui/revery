module Time = Revery_Core.Time;
module Spring = Revery_UI.Spring;

let spring = (~target, ~restThreshold=0.1, options) => {
  let%hook (previousState, setPreviousState) =
    Ref.ref(Spring.create(target, Time.now()));

  let isActive =
    !Spring.isAtRest(~restThreshold, previousState)
    || Float.abs(target -. previousState.position) > restThreshold;

  let%hook (time, _) = Timer.timer(~active=isActive, ());

  let state = Spring.tick(target, previousState, options, time);
  setPreviousState(state);

  state.position
};
