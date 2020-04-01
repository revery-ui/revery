module SpringHook = Spring;

open Revery_UI;

include Effect;
include Reducer;
include Ref;
include State;
include Tick;

let time = Timer.time;
let timer = Timer.timer;

let animation = (~active=true, ~onComplete=() => (), animation) => {
  let%hook (isCompleted, setCompleted) = state(false);
  let%hook (time, resetTimer) = timer(~active=active && !isCompleted, ());

  let (value, animationState) = Animation.apply(time, animation);

  // Stop timer when animation completes
  switch (animationState) {
  | Complete(_) =>
    onComplete();
    setCompleted(_ => true);
  | _ => ()
  };

  let reset = () => {
    setCompleted(_ => false);
    resetTimer();
  };

  (value, animationState, reset);
};

let transition =
    (
      ~duration=Time.seconds(1),
      ~delay=Time.zero,
      ~easing=Easing.linear,
      ~initialValue=?,
      targetValue,
    ) => {

  let initialValue = switch(initialValue) {
  | Some(v) => v
  | None => targetValue
  };

  let specifiedTargetValue = targetValue;

  let%hook ((startValue, targetValue), internalSetTarget) =
    state((initialValue, targetValue));

  let anim =
    Animation.animate(duration)
    |> Animation.delay(delay)
    |> Animation.ease(easing)
    |> Animation.tween(startValue, targetValue);

  let%hook (value, _animationState, resetTimer) = animation(anim);

  let setTargetValue = newTarget => {
    resetTimer();
    internalSetTarget(_ => (value, newTarget));
  };

  let%hook () = effect(If((!=), specifiedTargetValue), () => {
    setTargetValue(specifiedTargetValue);
    None;
  });

  value
};

let spring = SpringHook.spring;
