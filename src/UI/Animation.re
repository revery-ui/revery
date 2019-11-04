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

  module AnimationId =
    UniqueId.Make({});

  type animation = {
    id: int,
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

  let addAnimation = anim => activeAnimations := [anim, ...activeAnimations^];

  let removeAnimation = (anim: animation) =>
    activeAnimations :=
      List.filter(
        ({animation: a, _}) => a.id !== anim.id,
        activeAnimations^,
      );

  let cancelAll = () => activeAnimations := [];

  let isActive = animation =>
    List.exists(a => a.animation.id == animation.id, activeAnimations^);

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
        ~duration=Time.Seconds(1.0),
        ~delay=Time.Seconds(0.0),
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
        optCall(complete, ());
      };
    } else {
      anim.value = interpolate(t, startValue, toValue);
      optCall(update, anim.value);
    };
  };

  let getAnimationCount = () => List.length(activeAnimations^);

  let anyActiveAnimations = () => {
    let t = Time.to_float_seconds(AnimationTickerImpl.time());
    let anims = List.filter(hasStarted(t), activeAnimations^);
    List.length(anims) > 0;
  };

  let tween =
      (initialValue: float, animationOptions: animationOptions) => {
    let animation = {
      id: AnimationId.getUniqueId(),
      delay: Time.to_float_seconds(animationOptions.delay),
      duration: Time.to_float_seconds(animationOptions.duration),
      toValue: animationOptions.toValue,
      repeat: animationOptions.repeat,
      value: initialValue,
      startTime: Time.to_float_seconds(AnimationTickerImpl.time()),
      initialValue,
      easing: animationOptions.easing,
      direction: animationOptions.direction,
      isReverse: false,
    };
    animation;
  };

  let start = (~update=?, ~complete=?, animation) => {
    let activeAnimation = {animation, update, complete};
    let isReverseStartValue =
      animation.direction == `Reverse
      || animation.direction == `AlternateReverse;
    animation.startTime = Time.to_float_seconds(AnimationTickerImpl.time());
    animation.isReverse = isReverseStartValue;
    addAnimation(activeAnimation);
    let playback = {
      pause: () => removeAnimation(activeAnimation.animation),
      stop: () => {
        animation.value = animation.initialValue;
        animation.isReverse = isReverseStartValue;
        removeAnimation(activeAnimation.animation);
      },
    };
    playback;
  };

  let getTime = () => AnimationTickerImpl.time();

  let restart = (~completer, animation) => {
    animation.startTime = Time.to_float_seconds(getTime());
    animation.value = animation.initialValue;
    let newActiveAnim = {
      animation,
      update: None,
      complete: Some(completer),
    };
    addAnimation(newActiveAnim);
  };

  let pause = (~completer, animation) => {
    removeAnimation(animation);

    let resume = () => {
      let newActiveAnim = {
        animation,
        update: None,
        complete: Some(completer),
      };
      addAnimation(newActiveAnim);
    };
    resume
  };

  let getValue = (animation) =>
    animation.value;

  let setValue = (value, animation) =>
    animation.value = value;

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

  let tick = (t: float) => {
    List.iter(tickAnimation(t), activeAnimations^);
    activeAnimations :=
      List.filter(a => !isComplete(t, a), activeAnimations^);
  };

  Event.subscribe(AnimationTickerImpl.onTick, t =>
    tick(Time.to_float_seconds(t))
  );
};
