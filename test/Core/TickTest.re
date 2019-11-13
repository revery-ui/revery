open Revery_Core;

open TestFramework;

module TestTicker = {
  let _time: ref(Time.t) = ref(Time.zero);

  let incrementTime = (time: Time.t) => {
    _time := Time.(_time^ + time);
  };

  let time = () => _time^;
};

module Tick = Revery_Core.Internal.Tick.Make(TestTicker);

describe("Ticker", ({describe, _}) => {
  describe("timeout", ({test, _}) => {
    test("calls once after tick time", ({expect, _}) => {
      let callCount = ref(0);
      let _ignore = Tick.timeout(() => incr(callCount), Time.seconds(1));
      TestTicker.incrementTime(Time.ms(1010));
      Tick.pump();

      expect.int(callCount^).toBe(1);

      // Incrementing again, the timeout should've expired - so no additional increment
      TestTicker.incrementTime(Time.ms(1010));
      Tick.pump();

      expect.int(callCount^).toBe(1);
    });
    test(
      "nested tick - tick gets scheduled when called from inside tick",
      ({expect, _}) => {
      let outerCallCount = ref(0);
      let innerCallCount = ref(0);

      let _ignore =
        Tick.timeout(
          () => {
            incr(outerCallCount);
            let _ignore =
              Tick.timeout(() => incr(innerCallCount), Time.ms(110));
            ();
          },
          Time.zero,
        );
      TestTicker.incrementTime(Time.ms(110));
      Tick.pump();

      expect.int(outerCallCount^).toBe(1);
      expect.int(innerCallCount^).toBe(0);

      TestTicker.incrementTime(Time.ms(110));
      Tick.pump();

      expect.int(outerCallCount^).toBe(1);
      expect.int(innerCallCount^).toBe(1);

      // Incrementing again, the timeout should've expired - so no additional increment
      TestTicker.incrementTime(Time.ms(1010));
      Tick.pump();

      expect.int(outerCallCount^).toBe(1);
      expect.int(innerCallCount^).toBe(1);
    });
    test("doesn't call if canceled", ({expect, _}) => {
      let callCount = ref(0);
      let cancel = Tick.timeout(() => incr(callCount), Time.seconds(1));
      TestTicker.incrementTime(Time.ms(500));
      Tick.pump();

      cancel();
      TestTicker.incrementTime(Time.ms(510));
      Tick.pump();

      expect.int(callCount^).toBe(0);
    });
  });
  describe("interval", ({test, _}) => {
    test("calls after tick time", ({expect, _}) => {
      let callCount = ref(0);

      let _ignore =
        Tick.interval(_ => callCount := callCount^ + 1, Time.seconds(1));

      TestTicker.incrementTime(Time.ms(1010));

      expect.int(callCount^).toBe(0);
      Tick.pump();
      expect.int(callCount^).toBe(1);

      Tick.pump();
      expect.int(callCount^).toBe(1);

      TestTicker.incrementTime(Time.ms(900));
      Tick.pump();
      expect.int(callCount^).toBe(1);

      TestTicker.incrementTime(Time.ms(110));
      Tick.pump();
      expect.int(callCount^).toBe(2);
    });

    test("disposing tick subscription stops the tick", ({expect, _}) => {
      let callCount = ref(0);

      let stop =
        Tick.interval(_ => callCount := callCount^ + 1, Time.seconds(1));

      TestTicker.incrementTime(Time.ms(1010));

      expect.int(callCount^).toBe(0);
      Tick.pump();
      expect.int(callCount^).toBe(1);

      stop();

      TestTicker.incrementTime(Time.seconds(2));
      Tick.pump();
      expect.int(callCount^).toBe(1);
    });
  });
});
