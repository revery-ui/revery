open Revery_Core;
open Revery_UI;
open Revery_UI_Primitives;

module Tick = Revery_Core.Tick;
module Hooks = Revery_UI_Hooks;

open TestFramework;

describe("Hooks", ({describe, _}) => {
  describe("Timer", ({test, _}) => {
    module SingleTimer = {
      let%component make = (~timerActive, ()) => {
        let%hook (_dt, _reset) = Hooks.timer(~active=timerActive, ());

        <View />;
      };
    };

    module DoubleTimer = {
      let%component make = (~timer1Active, ~timer2Active, ()) => {
        let%hook (_dt, _reset) = Hooks.timer(~active=timer1Active, ());
        let%hook (_dt, _reset) = Hooks.timer(~active=timer2Active, ());
        <View />;
      };
    };

    let getTickerCount = () => Tick.getActiveTickers() |> List.length;

    test("Single: Timer starts and stops", ({expect, _}) => {
      expect.int(getTickerCount()).toBe(0);

      let rootNode = (new viewNode)();
      let container = Container.create(rootNode);

      let container =
        Container.update(container, <SingleTimer timerActive=true />);
      Tick.pump();
      expect.int(getTickerCount()).toBe(1);
      let container =
        Container.update(container, <SingleTimer timerActive=false />);
      Tick.pump();
      expect.int(getTickerCount()).toBe(0);

      let container =
        Container.update(container, <SingleTimer timerActive=true />);
      Tick.pump();
      expect.int(getTickerCount()).toBe(1);

      let _container =
        Container.update(container, <SingleTimer timerActive=false />);
      Tick.pump();
      expect.int(getTickerCount()).toBe(0);
    });

    test("Single: Timer starts and stops, in between pumps", ({expect, _}) => {
      expect.int(getTickerCount()).toBe(0);

      let rootNode = (new viewNode)();
      let container = Container.create(rootNode);

      let container =
        Container.update(container, <SingleTimer timerActive=true />);
      Tick.pump();
      expect.int(getTickerCount()).toBe(1);

      let container =
        Container.update(container, <SingleTimer timerActive=false />);
      let container =
        Container.update(container, <SingleTimer timerActive=true />);

      let _container =
        Container.update(container, <SingleTimer timerActive=false />);
      Tick.pump();
      expect.int(getTickerCount()).toBe(0);
    });

    test("Double: Timer starts and stops", ({expect, _}) => {
      expect.int(getTickerCount()).toBe(0);

      let rootNode = (new viewNode)();
      let container = Container.create(rootNode);

      let container =
        Container.update(
          container,
          <DoubleTimer timer1Active=true timer2Active=true />,
        );
      Tick.pump();
      expect.int(getTickerCount()).toBe(2);
      let container =
        Container.update(
          container,
          <DoubleTimer timer1Active=false timer2Active=true />,
        );
      Tick.pump();
      expect.int(getTickerCount()).toBe(1);
      let container =
        Container.update(
          container,
          <DoubleTimer timer1Active=true timer2Active=false />,
        );
      Tick.pump();
      expect.int(getTickerCount()).toBe(1);

      // Without the 'hack' to force disposal of the timers in Timer.re,
      // this particular case fails - it will leaving a hanging timer.
      let _container =
        Container.update(
          container,
          <DoubleTimer timer1Active=false timer2Active=false />,
        );
      Tick.pump();
      expect.int(getTickerCount()).toBe(0);
    });
  });

  describe("Tick", ({test, _}) => {
    module Ticker = {
      let%component make = (~f, ()) => {
        let%hook () = Hooks.tick(~tickRate=Time.zero, f);

        <View />;
      };
    };

    test("Callback does not go stale", ({expect, _}) => {
      let rootNode = (new viewNode)();
      let container = Container.create(rootNode);
      let count = ref(0);

      let container =
        Container.update(container, <Ticker f={_ => count := 1} />);
      Tick.pump();
      expect.int(count^).toBe(1);

      let _container =
        Container.update(container, <Ticker f={_ => count := 2} />);
      Tick.pump();
      expect.int(count^).toBe(2);
    });
  });
});
