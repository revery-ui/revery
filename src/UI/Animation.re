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

  let activeAnimations: ref(list(animation)) = ref([]);

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

  let hasStarted = (clock: float, anim: animation) => {
    let t = getLocalTime(clock, anim);
    t > 0.;
  };

  let isComplete = (clock: float, anim: animation) => {
    let t = getLocalTime(clock, anim);
    t > 1. && !anim.repeat;
  };

  let tickAnimation = (clock: float, anim: animation) => {
    let t = anim.easing(getLocalTime(clock, anim));

    /* If the anim is set to repeat and the time has expired, restart */
    if (anim.repeat && t > 1.) {
      /* reset */

      anim.startTime = anim.startTime +. anim.delay +. anim.duration;
      let newT = getLocalTime(clock, anim);
      anim.value.current = interpolate(newT, anim.startValue, anim.toValue);
    } else {
      anim.value.current = interpolate(t, anim.startValue, anim.toValue);
    };
  };

  let getAnimationCount = () => List.length(activeAnimations^);

  let anyActiveAnimations = () => {
    let t = Time.to_float_seconds(AnimationTickerImpl.time());
    let anims = List.filter(hasStarted(t), activeAnimations^);
    List.length(anims) > 0;
  };

  let start =
      (animationValue: animationValue, animationOptions: animationOptions) => {
    let animation: animation = {
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

    activeAnimations := List.append([animation], activeAnimations^);
    animation;
  };

  let cancel = (anim: animation) =>
    activeAnimations := List.filter(a => a.id !== anim.id, activeAnimations^);

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
