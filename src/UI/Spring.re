/* Hooks specific to Revery */
module Tick_ = Tick;
open Revery_Core;

module Options = {
  type t = {
    stiffness: float,
    damping: float,
    initialValue: float,
  };

  let create = (~stiffness=180., ~damping=12., ~initialValue=0., ()) => {
    initialValue,
    damping,
    stiffness,
  };

  // Presets from react-spring: https://www.react-spring.io/docs/hooks/api
  let default = create(~stiffness=170., ~damping=26., ());
  let gentle = create(~stiffness=120., ~damping=14., ());
  let wobbly = create(~stiffness=180., ~damping=12., ());
  let stiff = create(~stiffness=210., ~damping=20., ());
  let slow = create(~stiffness=280., ~damping=60., ());
  let molasses = create(~stiffness=280., ~damping=120., ());
};

  type t = {
    // [position] is the current position of the 'mass' of the spring
    // aka, the current value
    position: float,
    velocity: float,
    acceleration: float,
    currentTime: Time.t,
    isActive: bool,
  };

  let create = (position: float, time: Time.t) => {
    position,
    velocity: 0.,
    acceleration: 0.,
    currentTime: time,
    isActive: true,
  };
  let _isActive = (acceleration, velocity) =>
    Float.abs(acceleration) >= 0.001
    || Float.abs(velocity) >= 0.001;


  let tick = (target: float, spring: t, options: Options.t, newTime: Time.t) => {
    let deltaT = Time.(newTime - spring.currentTime) |> Time.toFloatSeconds;
    if (deltaT > 0.) {
      let force = Float.abs(target -. spring.position) *. options.stiffness;
      let dir = spring.position > target ? (-1.) : 1.;

      let acceleration = dir *. force -. options.damping *. spring.velocity;
      let velocity = spring.velocity +. acceleration *. deltaT;
      let position = spring.position +. velocity *. deltaT;
      {acceleration, velocity, position, currentTime: newTime, isActive: _isActive(acceleration, velocity)};
    } else {
      { ...spring, currentTime: newTime, isActive: true }
    }
  };

  let isResting = v => !v.isActive;

  let toString  = (spring: t) =>
    Printf.sprintf(
      "x: %f v: %f a: %f",
      spring.position,
      spring.velocity,
      spring.acceleration,
    );

  let getPosition = spring => spring.position;
