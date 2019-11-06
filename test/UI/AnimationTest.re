open Revery_UI;
open Revery_Core;

open TestFramework;

module MakeTicker = (()) => {
  let _currentTime: ref(Time.t) = ref(Time.zero);

  let time = () => _currentTime^;

  let onTick: Event.t(Time.t) = Event.create();

  let simulateTick = (time: Time.t) => {
    _currentTime := time;
    Event.dispatch(onTick, time);
  };
};

describe("Animation", ({describe, test, _}) => {
  describe("floatValue", ({test, _}) =>
    test("Initial value set", ({expect, _}) => {
      module TestTicker =
        MakeTicker({});
      module Animated = Animation.Make(TestTicker);

      let myTestValue = Animated.floatValue(0.1);
      expect.float(myTestValue.current).toBeCloseTo(0.1);
    })
  );
  test("simple animation", ({expect, _}) => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);

    let myAnimation =
      Animated.tween(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(2.),
          delay: Time.zero,
          toValue: 10.,
          repeat: false,
          easing: Easing.linear,
          direction: `Normal,
        },
      );
    let _playback = Animated.start(myAnimation);

    TestTicker.simulateTick(Time.Seconds(1.));
    expect.float(myAnimation.value.current).toBeCloseTo(5.);
  });

  test("animation with quadratic easing", ({expect, _}) => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);

    let myAnimation =
      Animated.tween(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(1.),
          delay: Time.zero,
          toValue: 1.,
          repeat: false,
          easing: Easing.quadratic,
          direction: `Normal,
        },
      );
    let _playback = Animated.start(myAnimation);

    TestTicker.simulateTick(Time.Seconds(0.5));
    expect.float(myAnimation.value.current).toBeCloseTo(0.25);
  });

  test("animation that repeats", ({expect, _}) => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);

    let myAnimation =
      Animated.tween(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(2.),
          delay: Time.zero,
          toValue: 10.,
          repeat: true,
          easing: Easing.linear,
          direction: `Normal,
        },
      );
    let _playback = Animated.start(myAnimation);

    TestTicker.simulateTick(Time.Seconds(3.));
    expect.float(myAnimation.value.current).toBeCloseTo(5.);
  });

  test("animation with delay", ({expect, _}) => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);

    let myAnimation =
      Animated.tween(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(2.),
          delay: Time.Seconds(1.),
          toValue: 10.,
          repeat: false,
          easing: Easing.linear,
          direction: `Normal,
        },
      );
    let _playback = Animated.start(myAnimation);

    TestTicker.simulateTick(Time.Seconds(2.));
    expect.float(myAnimation.value.current).toBeCloseTo(5.);
  });

  test("animations are cleaned up", ({expect, _}) => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);

    let myAnimation =
      Animated.tween(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(1.),
          delay: Time.zero,
          toValue: 10.,
          repeat: false,
          easing: Easing.linear,
          direction: `Normal,
        },
      );
    let _playback = Animated.start(myAnimation);

    TestTicker.simulateTick(Time.Seconds(3.));

    expect.bool(Animated.anyActiveAnimations()).toBeFalse();
    expect.int(Animated.getAnimationCount()).toBe(0);
  });

  test("animations can be cancelled", ({expect, _}) => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);

    let myAnimation =
      Animated.tween(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(1.),
          delay: Time.zero,
          toValue: 10.,
          repeat: false,
          easing: Easing.linear,
          direction: `Normal,
        },
      );
    let {Animated.stop, _} = Animated.start(myAnimation);

    TestTicker.simulateTick(Time.Seconds(0.1));

    stop();

    expect.bool(Animated.anyActiveAnimations()).toBeFalse();
    expect.int(Animated.getAnimationCount()).toBe(0);
  });

  test("chained animations", ({expect, _}) => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);
    module Chain = Animated.Chain;

    let first =
      Animated.tween(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(2.),
          delay: Time.zero,
          toValue: 10.,
          repeat: false,
          easing: Easing.linear,
          direction: `Normal,
        },
      );
    let second =
      Animated.tween(
        Animated.floatValue(first.toValue),
        {
          duration: Time.Seconds(2.),
          delay: Time.zero,
          toValue: 0.,
          repeat: false,
          easing: Easing.linear,
          direction: `Normal,
        },
      );
    let _playback = Chain.make(first) |> Chain.add(second) |> Chain.start;

    TestTicker.simulateTick(Time.Seconds(1.));
    expect.float(first.value.current).toBeCloseTo(5.);
    // Simulate the end of the first, so the second can start
    TestTicker.simulateTick(Time.Seconds(2.));
    TestTicker.simulateTick(Time.Seconds(3.));
    expect.float(second.value.current).toBeCloseTo(5.);
  });

  test("chain animations can be cancelled", ({expect, _}) => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);
    module Chain = Animated.Chain;

    let first =
      Animated.tween(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(2.),
          delay: Time.zero,
          toValue: 10.,
          repeat: false,
          easing: Easing.linear,
          direction: `Normal,
        },
      );
    let second =
      Animated.tween(
        Animated.floatValue(first.toValue),
        {
          duration: Time.Seconds(2.),
          delay: Time.zero,
          toValue: 0.,
          repeat: false,
          easing: Easing.linear,
          direction: `Normal,
        },
      );
    let {Animated.stop, _} =
      Chain.make(first) |> Chain.add(second) |> Chain.start;

    TestTicker.simulateTick(Time.Seconds(0.1));

    stop();

    expect.bool(Animated.anyActiveAnimations()).toBeFalse();
    expect.int(Animated.getAnimationCount()).toBe(0);
  });

  let directionTest = (description, direction, before, after) => {
    test(
      description,
      ({expect, _}) => {
        module TestTicker =
          MakeTicker({});
        module Animated = Animation.Make(TestTicker);

        let myAnimation =
          Animated.tween(
            Animated.floatValue(0.),
            {
              duration: Time.Seconds(0.5),
              delay: Time.zero,
              toValue: 10.,
              repeat: true,
              easing: Easing.linear,
              direction,
            },
          );
        let _playback = Animated.start(myAnimation);

        expect.bool(myAnimation.isReverse).toBe(before);
        TestTicker.simulateTick(Time.Seconds(1.));
        expect.bool(myAnimation.isReverse).toBe(after);
      },
    );
  };

  directionTest("animation that does not alternate", `Normal, false, false);
  directionTest("animation that is in reverse", `Reverse, true, true);
  directionTest("animation that alternates", `Alternate, false, true);
  directionTest(
    "animation that alternates in reverse",
    `AlternateReverse,
    true,
    false,
  );

  test("animation resets when stopped", ({expect, _}) => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);

    let myAnimation =
      Animated.tween(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(0.5),
          delay: Time.zero,
          toValue: 10.,
          repeat: true,
          easing: Easing.linear,
          direction: `Alternate,
        },
      );
    let _playback = Animated.start(myAnimation);
    TestTicker.simulateTick(Time.Seconds(1.));
    _playback.stop();

    expect.float(myAnimation.value.current).toBeCloseTo(0.);
    expect.bool(myAnimation.isReverse).toBeFalse();
  });
});
