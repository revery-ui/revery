open Rejest;

open Revery_Core;
open Revery_Core.Time;

module TestTicker = {
  let _time: ref(Time.t) = ref(Seconds(0.));

  let incrementTime = (time: Time.t) => {
    _time := Time.increment(_time^, time);
  };

  let time = () => _time^;
};

module Tick = Revery_Core.Internal.Tick.Make(TestTicker);

test("Ticker", () => {
  test("calls after tick time", () => {
    let callCount = ref(0);

    let _ = Tick.interval(_ => callCount := callCount^ + 1, Seconds(1.));

    TestTicker.incrementTime(Seconds(1.01));

    expect(callCount^).toBe(0);
    Tick.pump();
    expect(callCount^).toBe(1);

    Tick.pump();
    expect(callCount^).toBe(1);

    TestTicker.incrementTime(Seconds(0.9));
    Tick.pump();
    expect(callCount^).toBe(1);

    TestTicker.incrementTime(Seconds(0.11));
    Tick.pump();
    expect(callCount^).toBe(2);
  });

  test("disposing tick subscription stops the tick", () => {
    let callCount = ref(0);

    let stop = Tick.interval(_ => callCount := callCount^ + 1, Seconds(1.));

    TestTicker.incrementTime(Seconds(1.01));

    expect(callCount^).toBe(0);
    Tick.pump();
    expect(callCount^).toBe(1);

    stop();

    TestTicker.incrementTime(Seconds(2.));
    Tick.pump();
    expect(callCount^).toBe(1);
  });
});
