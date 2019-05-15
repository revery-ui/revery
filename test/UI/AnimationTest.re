open Rejest;

open Revery_UI;
open Revery_Core;

module MakeTicker = (()) => {
  let _currentTime: ref(Time.t) = ref(Time.Seconds(0.));

  let time = () => _currentTime^;

  let onTick: Event.t(Time.t) = Event.create();

  let simulateTick = (time: Time.t) => {
    _currentTime := time;
    Event.dispatch(onTick, time);
  };
};

test("Animation", () => {
  test("floatValue", () =>
    test("Initial value set", () => {
      module TestTicker =
        MakeTicker({});
      module Animated = Animation.Make(TestTicker);

      let myTestValue = Animated.floatValue(0.1);
      expect(myTestValue.current).toBe(0.1);
    })
  );

  test("simple animation", () => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);

    let myAnimation =
      Animated.tween(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(2.),
          delay: Time.Seconds(0.),
          toValue: 10.,
          repeat: false,
          easing: Easing.linear,
          direction: `Normal,
        },
      );
    let _playback = Animated.start(myAnimation);

    TestTicker.simulateTick(Time.Seconds(1.));
    expect(myAnimation.value.current).toBe(5.);
  });

  test("animation with quadratic easing", () => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);

    let myAnimation =
      Animated.tween(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(1.),
          delay: Time.Seconds(0.),
          toValue: 1.,
          repeat: false,
          easing: Easing.quadratic,
          direction: `Normal,
        },
      );
    let _playback = Animated.start(myAnimation);

    TestTicker.simulateTick(Time.Seconds(0.5));
    expect(myAnimation.value.current).toBe(0.25);
  });

  test("animation that repeats", () => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);

    let myAnimation =
      Animated.tween(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(2.),
          delay: Time.Seconds(0.),
          toValue: 10.,
          repeat: true,
          easing: Easing.linear,
          direction: `Normal,
        },
      );
    let _playback = Animated.start(myAnimation);

    TestTicker.simulateTick(Time.Seconds(3.));
    expect(myAnimation.value.current).toBe(5.);
  });

  test("animation with delay", () => {
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
    expect(myAnimation.value.current).toBe(5.);
  });

  test("animations are cleaned up", () => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);

    let myAnimation =
      Animated.tween(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(1.),
          delay: Time.Seconds(0.),
          toValue: 10.,
          repeat: false,
          easing: Easing.linear,
          direction: `Normal,
        },
      );
    let _playback = Animated.start(myAnimation);

    TestTicker.simulateTick(Time.Seconds(3.));

    expect(Animated.anyActiveAnimations()).toBe(false);
    expect(Animated.getAnimationCount()).toBe(0);
  });

  test("animations can be cancelled", () => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);

    let myAnimation =
      Animated.tween(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(1.),
          delay: Time.Seconds(0.),
          toValue: 10.,
          repeat: false,
          easing: Easing.linear,
          direction: `Normal,
        },
      );
    let {Animated.stop, _} = Animated.start(myAnimation);

    TestTicker.simulateTick(Time.Seconds(0.1));

    stop();

    expect(Animated.anyActiveAnimations()).toBe(false);
    expect(Animated.getAnimationCount()).toBe(0);
  });

  test("chained animations", () => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);
    module Chain = Animated.Chain;

    let first =
      Animated.tween(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(2.),
          delay: Time.Seconds(0.),
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
          delay: Time.Seconds(0.),
          toValue: 0.,
          repeat: false,
          easing: Easing.linear,
          direction: `Normal,
        },
      );
    let _playback = Chain.make(first) |> Chain.add(second) |> Chain.start;

    TestTicker.simulateTick(Time.Seconds(1.));
    expect(first.value.current).toBe(5.);
    // Simulate the end of the first, so the second can start
    TestTicker.simulateTick(Time.Seconds(2.));
    TestTicker.simulateTick(Time.Seconds(3.));
    expect(second.value.current).toBe(5.);
  });

  test("chain animations can be cancelled", () => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);
    module Chain = Animated.Chain;

    let first =
      Animated.tween(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(2.),
          delay: Time.Seconds(0.),
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
          delay: Time.Seconds(0.),
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

    expect(Animated.anyActiveAnimations()).toBe(false);
    expect(Animated.getAnimationCount()).toBe(0);
  });

  let directionTest = (description, direction, before, after) => {
    test(
      description,
      () => {
        module TestTicker =
          MakeTicker({});
        module Animated = Animation.Make(TestTicker);

        let myAnimation =
          Animated.tween(
            Animated.floatValue(0.),
            {
              duration: Time.Seconds(0.5),
              delay: Time.Seconds(0.),
              toValue: 10.,
              repeat: true,
              easing: Easing.linear,
              direction,
            },
          );
        let _playback = Animated.start(myAnimation);

        expect(myAnimation.isReverse).toBe(before);
        TestTicker.simulateTick(Time.Seconds(1.));
        expect(myAnimation.isReverse).toBe(after);
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

  test("animation resets when stopped", () => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);

    let myAnimation =
      Animated.tween(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(0.5),
          delay: Time.Seconds(0.),
          toValue: 10.,
          repeat: true,
          easing: Easing.linear,
          direction: `Alternate,
        },
      );
    let _playback = Animated.start(myAnimation);
    TestTicker.simulateTick(Time.Seconds(1.));
    _playback.stop();

    expect(myAnimation.value.current).toBe(0.);
    expect(myAnimation.isReverse).toBe(false);
  });
});
