module type Clock = {let time: unit => Time.t;};

module Log = (val Log.withNamespace("Revery.Tick"));

module DefaultClock = {
  let time = Time.now;
};

type callback = Time.t => unit;
type dispose = unit => unit;

module IntMap =
  Map.Make({
    type t = int;
    let compare = compare;
  });

module Make = (ClockImpl: Clock) => {
  type nonrec callback = callback;
  module TickId =
    UniqueId.Make({});

  type tickType =
    | Timeout
    | Interval;

  type tickFunction = {
    tickType,
    id: int,
    lastExecutionTime: Time.t,
    frequency: Time.t,
    f: callback,
  };

  let showTickFunction = (v: tickFunction) => {
    string_of_int(v.id);
  };

  let _activeTickers: ref(list(tickFunction)) = ref([]);
  let _scheduledTickers: ref(list(tickFunction)) = ref([]);
  let _cancelledTickers: ref(IntMap.t(bool)) = ref(IntMap.empty);

  let getActiveTickers = () => {
    _activeTickers^ |> List.map(tf => tf.id);
  };

  let toString = () =>
    _activeTickers^
    |> List.fold_left(
         (prev, curr) => showTickFunction(curr) ++ ", " ++ prev,
         "",
       );

  let pump = () => {
    // Add any newly-scheduled tickers
    _activeTickers := List.concat([_scheduledTickers^, _activeTickers^]);
    _scheduledTickers := [];

    Log.tracef(m =>
      m(
        "Tick.pump - starting with %d active tickers.",
        List.length(_activeTickers^),
      )
    );

    // Clear any pending tickers
    let cancelled = _cancelledTickers^;
    _activeTickers :=
      List.fold_left(
        (prev, curr) => {
          switch (IntMap.find_opt(curr.id, cancelled)) {
          | None => [curr, ...prev]
          | Some(_) => prev
          }
        },
        [],
        _activeTickers^,
      );
    _cancelledTickers := IntMap.empty;

    let currentTime = ClockImpl.time();

    let f = (tf: tickFunction) => {
      let nextTime = Time.(tf.lastExecutionTime + tf.frequency);

      if (nextTime <= currentTime) {
        let elapsedTime = Time.(currentTime - tf.lastExecutionTime);
        ignore(tf.f(elapsedTime));
        switch (tf.tickType) {
        | Timeout => None
        | Interval => Some({...tf, lastExecutionTime: currentTime})
        };
      } else {
        Some(tf);
      };
    };
    let newTickers = _activeTickers^ |> List.filter_map(f);
    _activeTickers := newTickers;

    Log.tracef(m =>
      m(
        "Tick.pump - ending with %d active tickers.",
        List.length(_activeTickers^),
      )
    );
  };

  let _clear = (~name, id: int, ()) => {
    Log.tracef(m => m("Clearing interval/timeout: %s %d", name, id));
    _cancelledTickers := IntMap.add(id, true, _cancelledTickers^);
  };

  exception Stop;

  let interval = (~name: string, f: callback, frequency: Time.t) => {
    let id = TickId.getUniqueId();
    Log.tracef(m => m("Interval - starting timer: %s %d", name, id));

    let f = t => {
      Log.tracef(m => m("Interval - running timer: %s %d", name, id));
      try(f(t)) {
      | Stop => _clear(~name, id, ())
      };
    };

    let tf: tickFunction = {
      tickType: Interval,
      id,
      lastExecutionTime: ClockImpl.time(),
      frequency,
      f,
    };

    _scheduledTickers := [tf, ..._scheduledTickers^];
    _clear(~name, id);
  };

  let timeout = (~name, f, waitTime: Time.t) => {
    let id = TickId.getUniqueId();
    Log.tracef(m => m("Timeout - starting timer: %s %d", name, id));

    let f = _t => {
      Log.tracef(m => m("Timeout - running timer: %s %d", name, id));
      f();
    };

    let tf: tickFunction = {
      tickType: Timeout,
      id,
      lastExecutionTime: ClockImpl.time(),
      frequency: waitTime,
      f,
    };

    _scheduledTickers := [tf, ..._scheduledTickers^];
    _clear(~name, id);
  };
};

module Default = Make(DefaultClock);
