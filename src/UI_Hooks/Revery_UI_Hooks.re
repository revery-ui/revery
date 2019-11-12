open Revery_UI;

include Effect;
include Reducer;
include Ref;
include Spring;
include State;
include Tick;
include Timer;

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

let transition = (~duration=Time.seconds(1.), ~delay=Time.zero, startValue) => {
  let%hook ((startValue, targetValue), setTargetValue) =
    state((startValue, startValue));

  let anim =
    Animation.animate(duration)
    |> Animation.delay(delay)
    |> Animation.tween(startValue, targetValue);

  let%hook (value, _animationState, resetTimer) = animation(anim);

  let setTargetValue = newTarget => {
    resetTimer();
    setTargetValue(_ => (value, newTarget));
  };

  (value, setTargetValue);
};
