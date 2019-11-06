open Revery_Core;
open Revery_Math;

type t = Time.t => float;

type result =
  | Delayed
  | Running(float) // Returns a value in the interval [0., 1.]
  | Complete(Time.t); // Returns elapsed time

/**
 * `time` is assumed to start at 0
 */
let animate =
    (~repeat=false, duration, time) => {
  let time = Time.toSeconds(time);
  let duration = Time.toSeconds(duration);

  let normalizedTime =
    time /. duration;

  if (normalizedTime < 0.) {
    Delayed;
  } else if (!repeat && normalizedTime > 1.) {
    Complete(Time.seconds(duration));
  } else {
    Running(mod_float(normalizedTime, 1.));
  };
};

let delay = (delay, animate, time) => {
  let delay = Time.toSeconds(delay) ;
  let time = Time.toSeconds(time);

  if (delay > time) {
    Delayed
  } else {
    animate(Time.ofSeconds(time -. delay))
  };
};

let ease = (easing, animate, time) =>
  switch (animate(time)) {
  | Running(v) => Running(easing(v))
  | result => result
  };

let andThen = (current, ~next, time) =>
  switch (current(time)) {
  | Complete(elapsed) => next(Time.ofSeconds(Time.toSeconds(time) -. Time.toSeconds(elapsed)))
  | result => result
  };

let tween = (start, finish, animate, time) =>
  switch (animate(time)) {
  | Running(t) => Running(t |> interpolate(start, finish))
  | result => result
  };

let tween' = (start, finish, animate, time) =>
  switch (tween(start, finish, animate, time)) {
  | Delayed => start
  | Running(t) => t
  | Complete(_) => finish
  };

let defaultTimer = () => Time.now();

// These functions need to be wrapped in records because of the recursive type definition
type pauser = {pause: unit => resumer}
and resumer = {resume: unit => pauser};

let rec run =
        (
          ~timer as getTime=defaultTimer,
          ~startTime=getTime(),
          ~onComplete=() => (),
          ~onUpdate,
          animate,
        )
        : pauser => {
  let startTime = Time.toSeconds(startTime);
  let time = Time.toSeconds(getTime());
  let stop =
    Tick.interval(
      _dt =>
        switch (animate(Time.ofSeconds(time -. startTime))) {
        | Delayed => ()
        | Running(t) => onUpdate(t)
        | Complete(_) =>
          onUpdate(1.);
          onComplete();
          raise(Tick.Stop);
        },
      Time.seconds(0.),
    );

  let pause = () => {
    stop();
    let pauseTime = Time.toSeconds(getTime());

    let resume = () =>
      run(
        ~timer=getTime,
        ~startTime=Time.ofSeconds(pauseTime -. startTime),
        ~onComplete,
        ~onUpdate,
        animate,
      );

    {resume: resume};
  };

  {pause: pause};
};
