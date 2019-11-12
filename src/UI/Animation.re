open Revery_Core;
open Revery_Math;

module NormalizedTime = {
  type t = float;
  let fromFloat = t => Float.max(0., Float.min(1., t));
};

type t('a) = Time.t => ('a, state)

and state =
  | Delayed
  | Running
  | Complete(Time.t); // Elapsed time

let const = (constant, _time) => (constant, Complete(Time.zero));

/**
 * `time` is assumed to start at 0
 */
let animate = (duration, time) => {
  let normalizedTime = Time.(toSeconds(time / duration));

  if (normalizedTime < 0.) {
    (0., Delayed);
  } else if (normalizedTime > 1.) {
    (1., Complete(duration));
  } else {
    (normalizedTime, Running);
  };
};

let delay = (delay, animate, time) =>
  if (delay > time) {
    (fst(animate(Time.zero)), Delayed);
  } else {
    switch (animate(Time.(time - delay))) {
    | (value, Complete(elapsed)) => (
        value,
        Complete(Time.(elapsed + delay)),
      )
    | result => result
    };
  };

let repeat = (animate, time) =>
  switch (animate(time)) {
  | (_, Complete(elapsed)) =>
    let remainder = elapsed == Time.zero ? Time.zero : Time.(time mod elapsed);
    animate(remainder);

  | result => result
  };

let alternatingRepeat = (animate, time) =>
  switch (animate(time)) {
  | (_, Complete(elapsed)) =>
    let iteration = int_of_float(floor(Time.toSeconds(time)));
    let shouldReverse = iteration mod 2 != 0; // if not divisble by 2
    let remainder = elapsed == Time.zero ? Time.zero : Time.(time mod elapsed);
    animate(shouldReverse ? Time.(elapsed - remainder) : remainder);

  | result => result
  };

let map = (f, animate, time) =>
  switch (animate(time)) {
  | (t, state) => (f(t), state)
  };

let ease = (easing, animate) => map(easing, animate);

let tween = (start, finish, animate) =>
  map(interpolate(start, finish), animate);

let andThen = (current, ~next, time) =>
  switch (current(time)) {
  | (_, Complete(elapsed)) => next(Time.(time - elapsed))
  | result => result
  };

let zip = ((a, b), time) =>
  switch (a(time), b(time)) {
  | ((aValue, Delayed), (bValue, Delayed)) => ((aValue, bValue), Delayed)

  | ((aValue, Complete(aElapsed)), (bValue, Complete(bElapsed))) => (
      (aValue, bValue),
      Complete(Time.max(aElapsed, bElapsed)),
    )

  | ((aValue, _), (bValue, _)) => ((aValue, bValue), Running)
  };

let apply = (time, animate) => animate(time);

let valueAt = (time, animate) => fst(animate(time));

let stateAt = (time, animate) => snd(animate(time));