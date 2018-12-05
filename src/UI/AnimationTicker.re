/* Animated */

/* This creates a react-native like 'Animated' API */

module Event = Reactify.Event;

open Revery_Core;

let onTick: Event.t(Time.t) = Event.create();

let time = () => Time.getTime();

let tick = () => Event.dispatch(onTick, time());
