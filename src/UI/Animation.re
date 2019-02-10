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

module Make = (AnimationTickerImpl: AnimationTicker) => {
  type animationValue = {mutable current: float};

  module AnimationId =
    UniqueId.Make({});

  type animation = {
    id: int,
    delay: float,
    mutable startTime: float,
    duration: float,
    startValue: float,
    toValue: float,
    value: animationValue,
    repeat: bool,
    easing: float => float,
  };

  type activeAnimation = {
    animation,
    update: option(float => unit),
    complete: option(unit => unit),
  };

  type playback = {
    pause: unit => unit,
    stop: unit => unit,
  };

  let activeAnimations: ref(list(activeAnimation)) = ref([]);

  type animationOptions = {
    duration: Time.t,
    delay: Time.t,
    toValue: float,
    repeat: bool,
    easing: float => float,
  };

  // From http://greweb.me/2012/02/bezier-curve-based-easing-functions-from-concept-to-implementation/
  module Bezier = {
    let make = (mX1, mY1, mX2, mY2) => {
      let a = (aA1, aA2) => {
        1.0 -. 3.0 *. aA2 +. 3.0 *. aA1;
      };
      let b = (aA1, aA2) => {
        3.0 *. aA2 -. 6.0 *. aA1;
      };
      let c = aA1 => {
        3.0 *. aA1;
      };

      // Returns x(t) given t, x1, and x2, or y(t) given t, y1, and y2.
      let calcBezier = (aT, aA1, aA2) => {
        ((a(aA1, aA2) *. aT +. b(aA1, aA2)) *. aT +. c(aA1)) *. aT;
      };

      // Returns dx/dt given t, x1, and x2, or dy/dt given t, y1, and y2.
      let getSlope = (aT, aA1, aA2) => {
        3.0 *. a(aA1, aA2) *. aT *. aT +. 2.0 *. b(aA1, aA2) *. aT +. c(aA1);
      };

      let getTForX = aX => {
        // Newton raphson iteration
        let rec newton = (guess, attempts) => {
          let currentSlope = getSlope(guess, mX1, mX2);
          let goodEnough = currentSlope === 0.0;
          if (goodEnough || attempts === 4) {
            guess;
          } else {
            let currentX = calcBezier(guess, mX1, mX2) -. aX;
            let newGuess = guess -. currentX /. currentSlope;
            newton(newGuess, attempts + 1);
          };
        };
        newton(aX, 0);
      };
      let get = aX =>
        if (mX1 === mY1 && mX2 === mY2) {
          aX;
        } else if (aX <= 0.) {
          0.;
        } else if (aX >= 1.) {
          1.;
        } else {
          calcBezier(getTForX(aX), mY1, mY2);
        };
      get;
    };
  };

  let genOneDimBezierFunc = (u1: float, u2: float) => {
    let a = 3. *. (u1 -. u2) +. 1.;
    let b = (-6.) *. u1 +. 3. *. u2;
    let c = 3. *. u1;
    let ret = (t: float) => {
      let t2 = t ** 2.;
      a *. t2 *. t +. b *. t2 +. c *. t;
    };
    ret;
  };
  let linear = (t: float) => t;
  let quadratic = (t: float) => t *. t;
  let cubic = (t: float) => t *. t *. t;
  let cubicBezier = Bezier.make;
  // From https://developer.mozilla.org/en-US/docs/Web/CSS/timing-function#Keywords_for_common_cubic-bezier_timing_functions
  let ease = cubicBezier(0.25, 0.1, 0.25, 1.0);
  let easeIn = cubicBezier(0.42, 0.0, 1.0, 1.0);
  let easeOut = cubicBezier(0.0, 0.0, 0.58, 1.0);
  let easeInOut = cubicBezier(0.42, 0.0, 0.58, 1.0);

  let floatValue = (v: float) => {
    let ret = {current: v};
    ret;
  };

  let getLocalTime = (clock: float, anim: animation) => {
    let adjustedStart = anim.startTime +. anim.delay;
    let endTime = adjustedStart +. anim.duration;
    (clock -. adjustedStart) /. (endTime -. adjustedStart);
  };

  let hasStarted = (clock: float, {animation, _}) => {
    let t = getLocalTime(clock, animation);
    t > 0.;
  };

  let isComplete = (clock: float, {animation: anim, _}) => {
    let t = getLocalTime(clock, anim);
    t > 1. && !anim.repeat;
  };

  let tickAnimation = (clock: float, {animation: anim, update, complete}) => {
    let t = anim.easing(getLocalTime(clock, anim));

    if (t >= 1.) {
      if (anim.repeat) {
        /* If the anim is set to repeat and the time has expired, restart */
        anim.startTime = anim.startTime +. anim.delay +. anim.duration;
        let newT = getLocalTime(clock, anim);
        anim.value.current = interpolate(newT, anim.startValue, anim.toValue);
      } else {
        optCall(complete, ());
      };
    } else {
      anim.value.current = interpolate(t, anim.startValue, anim.toValue);
      optCall(update, anim.value.current);
    };
  };

  let getAnimationCount = () => List.length(activeAnimations^);

  let anyActiveAnimations = () => {
    let t = Time.to_float_seconds(AnimationTickerImpl.time());
    let anims = List.filter(hasStarted(t), activeAnimations^);
    List.length(anims) > 0;
  };

  let tween =
      (animationValue: animationValue, animationOptions: animationOptions) => {
    let animation = {
      id: AnimationId.getUniqueId(),
      delay: Time.to_float_seconds(animationOptions.delay),
      duration: Time.to_float_seconds(animationOptions.duration),
      toValue: animationOptions.toValue,
      repeat: animationOptions.repeat,
      value: animationValue,
      startTime: Time.to_float_seconds(AnimationTickerImpl.time()),
      startValue: animationValue.current,
      easing: animationOptions.easing,
    };
    animation;
  };

  let start = (~update=?, ~complete=?, animation) => {
    let activeAnimation = {animation, update, complete};
    activeAnimations := List.append([activeAnimation], activeAnimations^);
    let removeAnimation = l =>
      List.filter(({animation: a, _}) => a.id !== animation.id, l);
    let playback = {
      pause: () => {
        activeAnimations := removeAnimation(activeAnimations^);
      },
      stop: () => {
        animation.value.current = animation.startValue;
        activeAnimations := removeAnimation(activeAnimations^);
      },
    };
    playback;
  };

  module Chain = {
    type t = {animations: list(animation)};
    let make = animation => {animations: [animation]};
    let add = (animation, {animations: l}) => {
      animations: List.cons(animation, l),
    };
    let start = (~update=?, ~complete=?, {animations: l}) => {
      let currentPlayback = ref(None);
      let rec runAnimation = (animations, index) => {
        switch (animations) {
        | [a, ...xl] =>
          let playback =
            a |> start(~update?, ~complete=() => runAnimation(xl, index + 1));
          currentPlayback := Some(playback);
        | [] => optCall(complete, ())
        };
      };
      runAnimation(List.rev(l), 0);
      let playback = {
        pause: () => {
          switch (currentPlayback^) {
          | Some(p) => p.pause()
          | None => ()
          };
        },
        stop: () => {
          switch (currentPlayback^) {
          | Some(p) => p.stop()
          | None => ()
          };
        },
      };
      playback;
    };
  };

  let cancel = (anim: animation) =>
    activeAnimations :=
      List.filter(
        ({animation: a, _}) => a.id !== anim.id,
        activeAnimations^,
      );

  let cancelAll = () => activeAnimations := [];

  let tick = (t: float) => {
    List.iter(tickAnimation(t), activeAnimations^);

    activeAnimations :=
      List.filter(a => !isComplete(t, a), activeAnimations^);
  };

  Event.subscribe(AnimationTickerImpl.onTick, t =>
    tick(Time.to_float_seconds(t))
  );
};