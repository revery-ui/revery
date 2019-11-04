open Revery_Core;
open Revery_Math;

module type AnimationTicker = {
  let time: unit => Time.t;

  let onTick: Event.t(Time.t);
};

type animationDirection = [
  | `Normal
  | `Reverse
  | `Alternate
  | `AlternateReverse
];

module Make:
  (AnimationTickerImpl: AnimationTicker) =>
   {
    type animation;
    type playback = {
      pause: unit => unit,
      stop: unit => unit,
    };
    let isActive: animation => bool;
    type animationOptions = {
      duration: Revery_Core.Time.t,
      delay: Revery_Core.Time.t,
      toValue: float,
      repeat: bool,
      easing: float => float,
      direction: animationDirection,
    };
    let options:
      (
        ~duration: Revery_Core.Time.t=?,
        ~delay: Revery_Core.Time.t=?,
        ~repeat: bool=?,
        ~easing: float => float=?,
        ~direction: animationDirection=?,
        ~toValue: float,
        unit
      ) =>
      animationOptions;
    let anyActiveAnimations: unit => bool;
    let tween: (float, animationOptions) => animation;
    let start:
      (~update: float => unit=?, ~complete: unit => unit=?, animation) =>
      playback;
    let restart: (~completer: unit => unit, animation) => unit;
    let pause: (~completer: unit => unit, animation) => (unit => unit);
    let cancelAll: unit => unit;

    let getValue: animation => float;
    let setValue: (float, animation) => unit;

    module Chain: {
      type t = {animations: list(animation)};
      let make: animation => t;
      let add: (animation, t) => t;
      let start:
        (~update: float => unit=?, ~complete: unit => unit=?, t) => playback;
    };
  };