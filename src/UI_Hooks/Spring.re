/* Hooks specific to Revery */
module Tick_ = Tick;
open Revery_Core;

module Tick = Tick_;

module Spring = {
  module Options = {
    type t = {
      initialValue: float,
      damping: float,
      springConstant: float,
    };

    let create = (~damping=12., ~springConstant=180., initialValue) => {
      initialValue,
      damping,
      springConstant,
    };
  };

  type t = {
    // [position] is the current position of the 'mass' of the spring
    // aka, the current value
    position: float,
    velocity: float,
    acceleration: float,
  };

  let create = (position: float) => {position, velocity: 0., acceleration: 0.};

  let tick = (spring: t, options: Options.t, deltaT: float) => {
    let force =
      Float.abs(spring.position -. options.initialValue) *. options.springConstant;
    let dir = spring.position > options.initialValue ? (-1.) : 1.;

    let acceleration = dir *. force -. options.damping *. spring.velocity;
    let velocity = spring.velocity +. acceleration *. deltaT;
    let position = spring.position +. velocity *. deltaT;
    {acceleration, velocity, position};
  };

  let show = (v: t) => Printf.sprintf("x: %f v: %f a: %f", v.position, v.velocity, v.acceleration);

  let isActive = (spring: t) =>
    Float.abs(spring.acceleration) >= 0.001 || Float.abs(spring.velocity) >= 0.001;
};

type state = {
  options: Spring.Options.t,
  spring: Spring.t,
  isActive: bool,
};

let initialState = (springVal, options) => {
  options,
  spring: Spring.create(springVal),
  isActive: true,
};

type action =
  | Reset(float)
  | ResetOptions(Spring.Options.t)
  | Tick(float);

let springReducer: (action, state) => state =
  (action, state) =>
    switch (action) {
    | Tick(time) when state.isActive =>
      let spring = Spring.tick(state.spring, state.options, time);
      {...state, spring, isActive: Spring.isActive(spring)};
    | Reset(springVal) => {
        ...state,
        isActive: true,
        spring: Spring.create(springVal),
      }
    | ResetOptions(options) => {...state, isActive: true, options}
    | _ => state
    };

let spring = (v: float, opts: Spring.Options.t) => {

  let%hook (curr, dispatch) =
    Reducer.reducer(~initialState=initialState(v, opts), springReducer);

  let%hook () = Tick.tick(~tickRate=Time.zero, dt => dispatch(Tick(Time.toSeconds(dt))));

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
      }
    );

  (curr.spring.position);
};
