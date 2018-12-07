open Revery_UI;
open Revery_Core;

open TestFramework;

module MakeTicker = (()) => {
  let _currentTime: ref(Time.t) = ref(Time.Seconds(0.));

  let time = () => _currentTime^;

  let onTick: Event.t(Time.t) = Event.create();

  let simulateTick = (time: Time.t) => {
    _currentTime := time;
    Event.dispatch(onTick, time);
  };
};

describe("Animation", ({describe, _}) => {
  describe("floatValue", ({test, _}) =>
    test("Initial value set", ({expect}) => {
      module TestTicker =
        MakeTicker({});
      module Animated = Animation.Make(TestTicker);

      let myTestValue = Animated.floatValue(0.1);
      expect.float(myTestValue.current).toBe(0.1);
    })
  );
  test("simple animation", ({expect}) => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);

    let myAnimation =
      Animated.start(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(2.),
          delay: Time.Seconds(0.),
          toValue: 10.,
          repeat: false,
        },
      );

    TestTicker.simulateTick(Time.Seconds(1.));
    expect.float(myAnimation.value.current).toBe(5.);
  });

  test("animation that repeats", ({expect}) => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);

    let myAnimation =
      Animated.start(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(2.),
          delay: Time.Seconds(0.),
          toValue: 10.,
          repeat: true,
        },
      );

    TestTicker.simulateTick(Time.Seconds(3.));
    expect.float(myAnimation.value.current).toBe(5.);
  });

  test("animations are cleaned up", ({expect}) => {
    module TestTicker =
      MakeTicker({});
    module Animated = Animation.Make(TestTicker);

    let _myAnimation =
      Animated.start(
        Animated.floatValue(0.),
        {
          duration: Time.Seconds(1.),
          delay: Time.Seconds(0.),
          toValue: 10.,
          repeat: false,
        },
      );

    TestTicker.simulateTick(Time.Seconds(3.));

    expect.bool(Animated.anyActiveAnimations()).toBe(false);
    expect.int(Animated.getAnimationCount()).toBe(0);
  });
});
