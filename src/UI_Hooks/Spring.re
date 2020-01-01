module Time = Revery_Core.Time;
module SpringModel = Revery_UI.Spring;

type state = {
  target: float,
  spring: SpringModel.t,
  isActive: bool,
};

let initialState = (springVal, time) => {
  target: springVal,
  spring: SpringModel.create(springVal, time),
  isActive: true,
};

type action =
  | Reset(float)
  | ResetOptions(SpringModel.Options.t)
  | Tick(Time.t);

let springReducer = (options, action, state) =>
  switch (action) {
  | Tick(time) when state.isActive =>
    let spring = SpringModel.tick(state.target, state.spring, options, time);
    {...state, spring, isActive: !SpringModel.isResting(spring)};
  | Reset(springVal) => {...state, target: springVal, isActive: true}
  | ResetOptions(options) => {...state, isActive: true}
  | _ => state
  };

let spring = (value, options) => {
  let%hook (state, dispatch) =
    Reducer.reducer(
      ~initialState=initialState(value, Time.now()),
      springReducer(options),
    );

  let%hook (time, _) =
    Timer.timer(~active=state.isActive, ~tickRate=Time.zero, ());

  if (state.isActive) {
    dispatch(Tick(time));
  };

  let%hook () =
    Effect.effect(
      If((!=), value),
      () => {
        dispatch(Reset(value));
        None;
      },
    );

  SpringModel.getPosition(state.spring);
};
