open Revery_UI;
open Revery_Core;
open Revery_UI_Primitives;

module Hooks = Revery_UI_Hooks;

type tickFunction = Time.t => unit;
let noop: tickFunction = _ => ();

let%component make =
              (
                ~children=React.empty,
                ~onTick=noop,
                ~tickRate=Time.seconds(1),
                ~name="<Ticker />",
                (),
              ) => {
  let%hook () =
    Hooks.effect(
      OnMount,
      () => {
        let dispose = Revery_Core.Tick.interval(~name, onTick, tickRate);

        Some(dispose);
      },
    );

  <View> children </View>;
};
