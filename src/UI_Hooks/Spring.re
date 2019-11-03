/* Hooks specific to Revery */
open Revery_Core;
open Revery_UI.Animated;

let reducer = (_a, s) => s + 1;

module Spring = {
  module Options = {
    type t = {
      initialValue: float,
      damping: float,
      springConstant: float,
    };

    let create = (~damping=10., ~springConstant=160., initialValue) => {
      initialValue,
      damping,
      springConstant,
    };
  };

  type t = {
    x: float,
    v: float,
    a: float,
  };

  let create = (x: float) => {x, v: 0., a: 0.};

  let tick = (spring: t, options: Options.t, deltaT: float) => {
    let force =
      Float.abs(spring.x -. options.initialValue) *. options.springConstant;
    let dir = spring.x > options.initialValue ? (-1.) : 1.;

    let a = dir *. force -. options.damping *. spring.v;
    let v = spring.v +. a *. deltaT;
    let x = spring.x +. v *. deltaT;
    {a, v, x};
  };

  let show = (v: t) => Printf.sprintf("x: %f v: %f a: %f", v.x, v.v, v.a);

  let isActive = (spring: t) =>
    Float.abs(spring.a) >= 0.001 || Float.abs(spring.v) >= 0.001;
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

let reducer: (action, state) => state =
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

let spring = (v: float, opts: Spring.Options.t, hooks) => {
  let (curr, dispatch, hooks) =
    Reducer.reducer(~initialState=initialState(v, opts), reducer, hooks);

  let hooks =
    Effect.effect(
      If((!=), v),
      () => {
        dispatch(Reset(v));
        None;
      },
      hooks,
    );

  let hooks =
    Effect.effect(
      If((!=), opts),
      () => {
        dispatch(ResetOptions(opts));
        None;
      },
      hooks,
    );

  let slots =
    Effect.effect(
      OnMount,
      () => {
        let dispose =
          Revery_Core.Tick.interval(
            dt => {dispatch(Tick(Revery_Core.Time.toSeconds(dt)))},
            Revery_Core.Time.Seconds(0.),
          );
        Some(dispose);
      },
      hooks,
    );

  (curr.spring.x, slots);
};
