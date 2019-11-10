open Revery_Core;
open Revery_Math;

type t('a) = Time.t => ('a, state)

and normalizedTime = float

and state =
  | Delayed
  | Running(normalizedTime)
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
    (normalizedTime, Running(normalizedTime));
  };
};

let delay = (delay, animate, time) =>
  if (delay > time) {
    (fst(animate(Time.zero)), Delayed);
  } else {
    animate(Time.(time - delay));
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

  | ((aValue, Running(aElapsed)), (bValue, Running(bElapsed))) => (
      (aValue, bValue),
      Running((aElapsed +. bElapsed) /. 2.),
    )

  | ((aValue, Complete(aElapsed)), (bValue, Complete(bElapsed))) => (
      (aValue, bValue),
      Complete(Time.max(aElapsed, bElapsed)),
    )

  | ((aValue, Complete(_)), (bValue, Running(elapsed)))
  | ((aValue, Running(elapsed)), (bValue, Complete(_))) => (
      (aValue, bValue),
      Running((1. +. elapsed) /. 2.),
    )

  | ((aValue, Delayed), (bValue, Running(elapsed)))
  | ((aValue, Running(elapsed)), (bValue, Delayed)) => (
      (aValue, bValue),
      Running(elapsed /. 2.),
    )

  | ((aValue, Delayed), (bValue, Complete(_)))
  | ((aValue, Complete(_)), (bValue, Delayed)) => (
      (aValue, bValue),
      Running(0.5),
    )
  };

let apply = (time, animate) => animate(time);
