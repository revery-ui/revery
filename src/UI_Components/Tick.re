
open Revery_UI;
open Revery_Core;
open Revery_Math;
open Revery_UI_Primitives;

module Hooks = Revery_UI_Hooks;

type tickFunction = Time.t => unit;
let noop: tickFunction = (_) => ();

let component = React.component("Container");

let createElement =
    (~children, ~onTick=noop, ~tickRate, ()) =>
  component(hooks => {

	let hooks = Hooks.effect(OnMount, (t) => {
		let dispose = Tick.interval(onTick, tickRate);

		Some(dispose);
	}, hooks);

    (hooks, <View> ...children </View>);
});
