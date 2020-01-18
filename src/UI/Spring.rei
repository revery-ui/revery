open Revery_Core;

module Options: {
  type t = {
    stiffness: float,
    damping: float,
  };

  let create: (~stiffness: float=?, ~damping: float=?, unit) => t;

  // Some basic presets
  let default: t;
  let gentle: t;
  let wobbly: t;
  let stiff: t;
  let slow: t;
  let molasses: t;
};

type t = {
  value: float,
  velocity: float,
  acceleration: float,
  time: Time.t,
};

// [create(position, time)] creates a new spring model with a mass at position of [position]
// starting at time [time].
let create: (float, Time.t) => t;

// [tick(targetPosition, spring, options, time)] calculates the updated spring model
// based on the [targetPosition], the current [spring] state, the spring [options],
// and the current absolute [time].
let tick: (float, t, Options.t, Time.t) => t;

let toString: t => string;

// isResting[spring] returns whether the spring is in a restng state
let isAtRest: (~restThreshold: float=?, t) => bool;

let setPosition: (float, t) => t;
