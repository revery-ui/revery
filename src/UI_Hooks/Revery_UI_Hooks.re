open Revery_UI;

include Effect;
include Reducer;
include Ref;
include State;
include Tick;

let time = (~tickRate=Time.zero, ()) => {
  let%hook (time, setTime) = reducer(~initialState=Time.now(), t => t);
  let%hook () = tick(~tickRate, _dt => setTime(_t => Time.now()));

  time;
};

// TODO: Workaround for https://github.com/briskml/brisk-reconciler/issues/27
// Remove when fixed
let mountIfEffect = (condition, handler) => {
  let%hook (maybeDispose, setDispose) = Ref.ref(None);
  let mountCleanup = () =>
    switch (maybeDispose) {
    | Some(dispose) =>
      dispose();
      setDispose(None);
    | None => ()
    };

  let%hook () =
    effect(
      OnMount,
      () => {
        setDispose(handler());
        Some(mountCleanup);
      },
    );

  let%hook () =
    effect(
      condition,
      () => {
        mountCleanup();
        handler();
      },
    );
  ();
};

let timer = (~tickRate=Time.zero, ~active=true, ()) => {
  let%hook (time, setTime) = reducer(~initialState=Time.now(), t => t);
  let%hook (startTime, setStartTime) = Ref.ref(time);

  let onTick = _dt => setTime(_t => Time.now());

  let%hook () =
    mountIfEffect(If((!=), active), () =>
      if (active) {
        setStartTime(Time.(now() - (time - startTime)));
        let dispose = Revery_Core.Tick.interval(onTick, tickRate);
        Some(dispose);
      } else {
        None;
      }
    );

  let reset = () => {
    setStartTime(time);
  };

  (Time.(time - startTime), reset);
};

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
      startValue,
    ) => {
  let%hook ((startValue, targetValue), setTargetValue) =
    state((startValue, startValue));

  let anim =
    Animation.animate(duration)
    |> Animation.delay(delay)
    |> Animation.ease(easing)
    |> Animation.tween(startValue, targetValue);

  let%hook (value, _animationState, resetTimer) = animation(anim);

  let setTargetValue = newTarget => {
    resetTimer();
    setTargetValue(_ => (value, newTarget));
  };

  (value, setTargetValue);
};
