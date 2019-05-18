open Revery_Core;
open Revery_UI;

type tickFunction = Time.t => unit;

/**
{2 Description:}

The [Ticker] component is a helper that calls a specific function periodically as a side-effect.

{2 Usage:}

{[
let tick tick = (t) => print_endline("Time: " ++ string_of_float(Time.toSeconds(t)));

<Ticker onTick=tick tickRate=Seconds(1.0) />
]}

@param [onTick] Callback function, called for each tick.
@param [tickRate] Frequency of calling the [onTick] function.
*/
let createElement:
  (
    ~children: list(React.syntheticElement),
    ~onTick: tickFunction=?,
    ~tickRate: Time.t=?,
    unit
  ) =>
  React.syntheticElement;
