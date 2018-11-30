open Rejest;

open Revery_UI;
open Revery_Core;

module MakeTicker = () => {

    let _currentTime: ref(Animation.Time.t) = ref(Animation.Time.Seconds(0.));

    let time = () => _currentTime^;

    let onTick: Event.t(Animation.Time.t) = Event.create();

    let simulateTick = (time: Animation.Time.t)  => {
        _currentTime := time;
        Event.dispatch(onTick, time);
    };
};

test("Animation", () => {
  test("Math", () => {
      test("clamp", () => {
         module TestTicker = MakeTicker();
         module Animated = Animation.Make(TestTicker);

         expect(Animated.clamp(-1., 0., 1.)).toBe(0.) ;
         expect(Animated.clamp(2., 0., 1.)).toBe(1.) ;
         expect(Animated.clamp(0.5, 0., 1.)).toBe(0.5) ;
      });

      test("interpolate", () => {
          module TestTicker = MakeTicker();
          module Animated = Animation.Make(TestTicker);

          expect(Animated.interpolate(0., 100., 200.)).toBe(100.);
          expect(Animated.interpolate(0.5, 100., 200.)).toBe(150.);
          expect(Animated.interpolate(1., -2., 0.)).toBe(0.);
      });
  });
  test("floatValue", () => {
      test("Initial value set", () => {

          module TestTicker = MakeTicker();
          module Animated = Animation.Make(TestTicker);

          let myTestValue = Animated.floatValue(0.1);
          expect(myTestValue.current).toBe(0.1);
      });
  });
  test("simple animation", () => {
       module TestTicker = MakeTicker();
       module Animated = Animation.Make(TestTicker);

       let myAnimation = Animated.start(Animated.floatValue(0.), {
        duration: Animation.Time.Seconds(2.),
        delay: Animation.Time.Seconds(0.),
        toValue: 10.,
        repeat: false,
       });

       TestTicker.simulateTick(Animation.Time.Seconds(1.));
       expect(myAnimation.value.current).toBe(5.);
  });

  test("animation that repeats", () => {
       module TestTicker = MakeTicker();
       module Animated = Animation.Make(TestTicker);

       let myAnimation = Animated.start(Animated.floatValue(0.), {
        duration: Animation.Time.Seconds(2.),
        delay: Animation.Time.Seconds(0.),
        toValue: 10.,
        repeat: true,
       });

       TestTicker.simulateTick(Animation.Time.Seconds(3.));
       expect(myAnimation.value.current).toBe(5.);
  });

  test("animations are cleaned up", () => {
       module TestTicker = MakeTicker();
       module Animated = Animation.Make(TestTicker);

       let _myAnimation = Animated.start(Animated.floatValue(0.), {
        duration: Animation.Time.Seconds(1.),
        delay: Animation.Time.Seconds(0.),
        toValue: 10.,
        repeat: false,
       });

       TestTicker.simulateTick(Animation.Time.Seconds(3.));

       expect(Animated.anyActiveAnimations()).toBe(false);
       expect(Animated.getAnimationCount()).toBe(0);
  });
});
