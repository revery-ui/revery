module Time = Revery_Core.Time;
module SpringModel = Revery_UI.Spring;

let spring = (target, options) => {
  let%hook (previousState, setPreviousState) =
    Ref.ref(SpringModel.create(target, Time.now()));

  let isActive = !SpringModel.isResting(target, previousState);

  let%hook (time, _) = Timer.timer(~active=isActive, ());

  let state = SpringModel.tick(target, previousState, options, time);
  setPreviousState(state);

  SpringModel.getPosition(state);
};
