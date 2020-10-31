module SpringHook = Spring;

open Revery_UI;

include Effect;
include Reducer;
include Ref;
include State;
include Tick;

let time = Timer.time;
let timer = Timer.timer;

let animation = (~active=true, ~onComplete=() => (), ~name, animation) => {
  let%hook (isCompleted, setCompleted) = state(false);
  let%hook (time, resetTimer) =
    timer(~name, ~active=active && !isCompleted, ());

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

module Internal = {
  type durationCalculation =
    | Constant(Time.t)
    | Function({
        initial: Time.t,
        calculated: (~current: float, ~target: float) => Time.t,
      });

  let transition =
      (
        ~active=true,
        ~duration: durationCalculation,
        ~delay=Time.zero,
        ~easing=Easing.linear,
        ~initialValue=?,
        ~name,
        specifiedTargetValue,
      ) => {
    let initialValue =
      switch (initialValue) {
      | Some(v) => v
      | None => specifiedTargetValue
      };

    let initialDurationTime =
      switch (duration) {
      | Constant(time) => time
      | Function({initial, _}) => initial
      };

    let%hook ((startValue, targetValue, durationTime), internalSetTarget) =
      state((initialValue, specifiedTargetValue, initialDurationTime));

    let anim =
      Animation.animate(durationTime)
      |> Animation.delay(delay)
      |> Animation.ease(easing)
      |> Animation.tween(startValue, targetValue);

    let%hook (value, _animationState, resetTimer) =
      animation(~name, ~active, anim);

    let value = active ? value : specifiedTargetValue;

    let setTargetValue = (newTarget, durationTime) => {
      resetTimer();
      internalSetTarget(_ => (value, newTarget, durationTime));
    };

    let%hook () =
      effect(
        If((!=), specifiedTargetValue),
        () => {
          let durationTime =
            switch (duration) {
            | Function({calculated, _}) =>
              calculated(~current=value, ~target=specifiedTargetValue)
            | Constant(time) => time
            };

          setTargetValue(specifiedTargetValue, durationTime);
          None;
        },
      );

    value;
  };
};

let transition =
    (
      ~active=true,
      ~duration=Time.seconds(1),
      ~delay=Time.zero,
      ~easing=Easing.linear,
      ~initialValue=?,
      targetValue,
    ) =>
  Internal.transition(
    ~active,
    ~duration=Internal.Constant(duration),
    ~delay,
    ~easing,
    ~initialValue?,
    targetValue,
  );

let transitionf =
    (
      ~active=true,
      ~delay=Time.zero,
      ~easing=Easing.linear,
      ~initialDuration,
      ~duration,
      ~initialValue=?,
      targetValue,
    ) =>
  Internal.transition(
    ~active,
    ~duration=
      Internal.Function({initial: initialDuration, calculated: duration}),
    ~delay,
    ~easing,
    ~initialValue?,
    targetValue,
  );

let spring = SpringHook.spring;

/**
 * let%hook (captureMouse, captureState) = Hooks.mouseCapture(...);
 *
 *   where [captureMouse] requires the initial state to be passed as its argument.
 *
 * Each callback is passed the current state and event data, and requires either a
 * state value to be returned or [None] to stop the capture.
 */
let mouseCapture =
    (
      ~onMouseDown=(state, _evt) => Some(state),
      ~onMouseUp=(state, _evt) => Some(state),
      ~onMouseMove=(state, _evt) => Some(state),
      ~onMouseWheel=(state, _evt) => Some(state),
      ~onRelease=_state => (),
      (),
    ) => {
  let%hook state = ref(None);

  let%hook () =
    effect(OnMount, () =>
      Some(
        () =>
          if (state^ != None) {
            Mouse.releaseCapture();
          },
      )
    );

  let capture = initialState => {
    state := Some(initialState);

    let wrap = (f, event) => {
      state := f(Option.get(state^), event);

      if (state^ == None) {
        Mouse.releaseCapture();
      };
    };

    Mouse.setCapture(
      Revery_UI.getActiveWindow() |> Option.get, // May fail if called outside rendering
      ~onMouseDown=wrap(onMouseDown),
      ~onMouseUp=wrap(onMouseUp),
      ~onMouseMove=wrap(onMouseMove),
      ~onMouseWheel=wrap(onMouseWheel),
      ~onRelease=() =>
      onRelease(state^)
    );
  };

  (capture, state^);
};

let hover = () => {
  let%hook (isHovered, setHovered) = state(false);

  let startHover = _evt => setHovered(_ => true);
  let endHover = _evt => setHovered(_ => false);

  (isHovered, startHover, endHover);
};
