module Time = Revery_Core.Time;
module SpringModel = Revery_UI.Spring;

type state = {
  target: float,
  options: SpringModel.Options.t,
  spring: SpringModel.t,
  isActive: bool,
};

let initialState = (springVal, options, time) => {
  options,
  target: springVal,
  spring: SpringModel.create(options.initialValue, time),
  isActive: true,
};

type action =
  | Reset(float)
  | ResetOptions(SpringModel.Options.t)
  | Tick(Time.t);

let springReducer: (action, state) => state =
  (action, state) =>
    switch (action) {
    | Tick(time) when state.isActive =>
      let spring =
        SpringModel.tick(state.target, state.spring, state.options, time);
      {...state, spring, isActive: !SpringModel.isResting(spring)};
    | Reset(springVal) => {...state, target: springVal, isActive: true}
    | ResetOptions(options) => {...state, isActive: true, options}
    | _ => state
    };

let spring = (v: float, opts: SpringModel.Options.t) => {
  let%hook (curr, dispatch) =
    Reducer.reducer(
      ~initialState=initialState(v, opts, Time.now()),
      springReducer,
    );

  let%hook (time, _) =
    Timer.timer(~active=curr.isActive, ~tickRate=Time.zero, ());

  if (curr.isActive) {
    dispatch(Tick(time));
  };

  let%hook () =
    Effect.effect(
      If((!=), v),
      () => {
        dispatch(Reset(v));
        None;
      },
    );

  let%hook () =
    Effect.effect(
      If((!=), opts),
      () => {
        dispatch(ResetOptions(opts));
        None;
      },
    );

  curr.spring |> SpringModel.getPosition;
};
