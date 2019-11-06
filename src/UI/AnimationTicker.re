/* Animated */

/* This creates a react-native like 'Animated' API */

open Revery_Core;

let onTick: Event.t(Time.t) = Event.create();

let time = () => Time.now();

let tick = () => Event.dispatch(onTick, time());
