open Revery_Core;

type tickFunction = Time.t => unit;

/**
{2 Description:}

The [Ticker] component is a helper that calls a specific function periodically as a side-effect.

{2 Usage:}

{[
let tick tick = (t) => print_endline("Time: " ++ string_of_float(Time.toFloatSeconds(t)));

<Ticker onTick=tick tickRate=Seconds(1.0) />
]}

@param [onTick] Callback function, called for each tick.
@param [tickRate] Frequency of calling the [onTick] function.
*/
let make:
  (
    ~key: Brisk_reconciler.Key.t=?,
    ~children: Revery_UI.element=?,
    ~onTick: tickFunction=?,
    ~tickRate: Revery_Core.Time.t=?,
    ~name: string=?,
    unit
  ) =>
  Brisk_reconciler.element(Revery_UI.viewNode);
