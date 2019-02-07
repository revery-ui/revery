open Revery_Core;
open Revery_Math;

module type AnimationTicker = {
  let time: unit => Time.t;

  let onTick: Event.t(Time.t);
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

  /*y=u0(1−t)3+3u1(1−t)2t+3u2(1−t)t2+u3t3
     u0, u3 = 0,1; t in [0,1]
     equivalent to a normal bezier curve where p1x = 1/3 and p2x = 2/3
    */
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
  let easeIn = genOneDimBezierFunc(0., 0.45);
  let easeOut = genOneDimBezierFunc(0.45, 1.);
  let easeInOut = genOneDimBezierFunc(0., 1.);

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

    if (t > 1.) {
      if (anim.repeat) {
        /* If the anim is set to repeat and the time has expired, restart */
        anim.startTime = anim.startTime +. anim.delay +. anim.duration;
        let newT = getLocalTime(clock, anim);
        anim.value.current = interpolate(newT, anim.startValue, anim.toValue);
      } else {
        switch (complete) {
        | Some(c) => c()
        | _ => ()
        };
      };
    } else {
      anim.value.current = interpolate(t, anim.startValue, anim.toValue);
      switch (update) {
      | Some(u) => u(anim.value.current)
      | _ => ()
      };
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
    type t = {
      first: animation,
      second: animation,
    };
    let make = (second, first) => {first, second};
    let start = (~update, ~complete, {first, second}) =>
      first
      |> start(~update, ~complete=() =>
           second |> start(~update, ~complete) |> ignore
         )
      |> ignore;
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