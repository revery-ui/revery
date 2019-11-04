open Revery_Core;
open Revery_Math;

module type AnimationTicker = {
  let time: unit => Time.t;

  let onTick: Event.t(Time.t);
};

let optCall = (opt, param) =>
  switch (opt) {
  | Some(f) => f(param)
  | None => ()
  };

type animationDirection = [
  | `Normal
  | `Reverse
  | `Alternate
  | `AlternateReverse
];

module Make = (AnimationTickerImpl: AnimationTicker) => {
  type animation = {
    delay: float,
    mutable startTime: float,
    duration: float,
    initialValue: float,
    toValue: float,
    mutable value: float,
    repeat: bool,
    easing: Easing.t,
    direction: animationDirection,
    mutable isReverse: bool,
    mutable running: bool,
    mutable update: option(float => unit),
    mutable complete: option(unit => unit),
  };

  type playback = {
    pause: unit => unit,
    stop: unit => unit,
  };

  type animationOptions = {
    duration: Time.t,
    delay: Time.t,
    toValue: float,
    repeat: bool,
    easing: float => float,
    direction: animationDirection,
  };

  let options =
      (
        ~duration=Time.seconds(1.0),
        ~delay=Time.zero,
        ~repeat=false,
        ~easing=Easing.linear,
        ~direction=`Normal,
        ~toValue: float,
        (),
      ) => {
    duration,
    delay,
    toValue,
    repeat,
    easing,
    direction,
  };

  let getLocalTime = (clock: float, anim: animation) => {
    let adjustedStart = anim.startTime +. anim.delay;
    let endTime = adjustedStart +. anim.duration;
    (clock -. adjustedStart) /. (endTime -. adjustedStart);
  };

  let tickAnimation = (clock: float, anim: animation) => {
    let t = anim.easing(getLocalTime(clock, anim));
    let (startValue, toValue) =
      anim.isReverse
        ? (anim.toValue, anim.initialValue) : (anim.initialValue, anim.toValue);

    if (t >= 1.) {
      if (anim.repeat) {
        if (anim.direction == `Alternate || anim.direction == `AlternateReverse) {
          anim.isReverse = !anim.isReverse;
        };

        /* If the anim is set to repeat and the time has expired, restart */
        anim.startTime = anim.startTime +. anim.delay +. anim.duration;

        let newT = getLocalTime(clock, anim);
        anim.value = interpolate(newT, startValue, toValue);
      } else {
        anim.value = interpolate(t, startValue, toValue);
        optCall(anim.update, anim.value);
        optCall(anim.complete, ());
        anim.running = false;
      };
    } else {
      anim.value = interpolate(t, startValue, toValue);
      optCall(anim.update, anim.value);
    };
  };

  let tween =
      (initialValue: float, animationOptions: animationOptions) => {
    let animation = {
      delay: Time.toSeconds(animationOptions.delay),
      duration: Time.toSeconds(animationOptions.duration),
      toValue: animationOptions.toValue,
      repeat: animationOptions.repeat,
      value: initialValue,
      startTime: Time.toSeconds(AnimationTickerImpl.time()),
      initialValue,
      easing: animationOptions.easing,
      direction: animationOptions.direction,
      isReverse: false,
      running: true,
      update: None,
      complete: None,
    };
    animation;
  };

  let start = (~update=?, ~complete=?, animation: animation) => {
    let isReverseStartValue =
      animation.direction == `Reverse
      || animation.direction == `AlternateReverse;
    animation.startTime = Time.toSeconds(AnimationTickerImpl.time());
    animation.isReverse = isReverseStartValue;
    animation.running = true;
    animation.update = update;
    animation.complete = complete;
    let playback = {
      pause: () => animation.running = false,
      stop: () => {
        animation.value = animation.initialValue;
        animation.isReverse = isReverseStartValue;
        animation.running = false;
      },
    };
    playback;
  };

  let getTime = () => AnimationTickerImpl.time();

  let restart = animation => {
    animation.startTime = Time.toSeconds(getTime());
    animation.value = animation.initialValue;
    animation.running = true;
  };

  let pause = (animation) => {
    animation.running = false

    let resume = () => {
      animation.running = true
    };
    resume
  };

  let getValue = (animation) => {
    if (animation.running) {
      let t = Time.toSeconds(getTime());
      tickAnimation(t, animation);
    }
    animation.value;
  };

  module Chain = {
    type t = {animations: list(animation)};
    let make = animation => {animations: [animation]};
    let add = (animation, {animations: l}) => {
      animations: List.cons(animation, l),
    };
    let start = (~update=?, ~complete=?, {animations: l}) => {
      let currentPlayback = ref(None);
      let rec runAnimation = (animations, index) =>
        switch (animations) {
        | [a, ...xl] =>
          let playback =
            a |> start(~update?, ~complete=() => runAnimation(xl, index + 1));
          currentPlayback := Some(playback);
        | [] => optCall(complete, ())
        };
      runAnimation(List.rev(l), 0);
      let playback = {
        pause: () =>
          switch (currentPlayback^) {
          | Some(p) => p.pause()
          | None => ()
          },
        stop: () =>
          switch (currentPlayback^) {
          | Some(p) => p.stop()
          | None => ()
          },
      };
      playback;
    };
  };
};
