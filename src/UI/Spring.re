/* Hooks specific to Revery */
open Revery_Core;

module Options = {
  type t = {
    stiffness: float,
    damping: float,
  };

  let create = (~stiffness=180., ~damping=12., ()) => {damping, stiffness};

  // Presets from react-spring: https://www.react-spring.io/docs/hooks/api
  let default = create(~stiffness=170., ~damping=26., ());
  let gentle = create(~stiffness=120., ~damping=14., ());
  let wobbly = create(~stiffness=180., ~damping=12., ());
  let stiff = create(~stiffness=210., ~damping=20., ());
  let slow = create(~stiffness=280., ~damping=60., ());
  let molasses = create(~stiffness=280., ~damping=120., ());
};

type t = {
  value: float,
  velocity: float,
  acceleration: float,
  time: Time.t,
};

let create = (value: float, time: Time.t) => {
  value,
  velocity: 0.,
  acceleration: 0.,
  time,
};

let tick = (target: float, spring: t, options: Options.t, time: Time.t) => {
  let deltaT = Time.(time - spring.time) |> Time.toFloatSeconds;
  if (deltaT > 0.) {
    // Cap the delta at 33 milliseconds / 30 FPS
    // This is important if the animation has been inactive!
    let deltaT = min(deltaT, 0.033);
    let force = Float.abs(target -. spring.value) *. options.stiffness;
    let dir = spring.value > target ? (-1.) : 1.;

    let acceleration = dir *. force -. options.damping *. spring.velocity;
    let velocity = spring.velocity +. acceleration *. deltaT;
    let value = spring.value +. velocity *. deltaT;

    {acceleration, velocity, value, time};
  } else {
    {...spring, time};
  };
};

let isAtRest = (~restThreshold=0.1, {acceleration, velocity, _}) =>
  Float.abs(acceleration) <= restThreshold
  && Float.abs(velocity) <= restThreshold;

let toString = (spring: t) =>
  Printf.sprintf(
    "x: %f v: %f a: %f",
    spring.value,
    spring.velocity,
    spring.acceleration,
  );

let setPosition = (value, state) => {
  ...state,
  value,
  velocity: 0.,
  acceleration: 0.,
};
