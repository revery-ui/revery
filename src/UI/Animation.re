open Revery_Core;
open Revery_Math;

type t('a) = Time.t => ('a, state)

and state =
  | Delayed
  | Running(float) // Elapsed time, normalized, i.e. in the range [0., 1.]
  | Complete(Time.t); // Elapsed time

/**
 * `time` is assumed to start at 0
 */
let animate = (duration, time) => {
  let time = Time.toSeconds(time);
  let duration = Time.toSeconds(duration);

  let normalizedTime =
    time /. duration;

  if (normalizedTime < 0.) {
    (0., Delayed);
  } else if (normalizedTime > 1.) {
    (1., Complete(Time.ofSeconds(duration)));
  } else {
    (normalizedTime, Running(normalizedTime));
  };
};

let delay = (delay, animate, time) => {
  let delay = Time.toSeconds(delay) ;
  let time = Time.toSeconds(time);

  if (delay > time) {
    (fst(animate(Time.ofSeconds(0.))), Delayed)
  } else {
    animate(Time.ofSeconds(time -. delay))
  };
};

let repeat = (animate, time) =>
  switch (animate(time)) {
  | (_, Complete(elapsed)) =>
    let elapsed = Time.toSeconds(elapsed);
    let time = Time.toSeconds(time);
    let remainder = elapsed == 0. ? 0. : mod_float(time, elapsed);
    animate(Time.ofSeconds(remainder))

  | result => result
  };

let ease = (easing, animate, time) =>
  switch (animate(time)) {
  | (t, state) => (easing(t), state)
  };

let andThen = (current, ~next, time) =>
  switch (current(time)) {
  | (_, Complete(elapsed)) => next(Time.ofSeconds(Time.toSeconds(time) -. Time.toSeconds(elapsed)))
  | result => result
  };

let tween = (start, finish, animate, time) =>
  switch (animate(time)) {
  | (t, state) => (interpolate(start, finish, t), state)
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
        | (_, Delayed) => ()
        | (t, Running(_)) => onUpdate(t)
        | (t, Complete(_)) =>
          onUpdate(t);
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
