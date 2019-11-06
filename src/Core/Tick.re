module type Clock = {let time: unit => Time.t;};

module DefaultClock = {
  let time = () => Time.getTime();
};

type callback = Time.t => unit;
type dispose = unit => unit;

module IntMap =
  Map.Make({
    type t = int;
    let compare = compare;
  });

module Make = (ClockImpl: Clock) => {
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

  let _filterMap = (v: list(option('a))): list('a) => {
    let rec f = v =>
      switch (v) {
      | [Some(hd), ...tail] => [hd, ...f(tail)]
      | [None, ...tail] => f(tail)
      | [] => []
      };

    f(v);
  };

  let show = () =>
    _activeTickers^
    |> List.fold_left(
         (prev, curr) => showTickFunction(curr) ++ ", " ++ prev,
         "",
       );

  let pump = () => {
    // Add any newly-scheduled tickers
    _activeTickers := List.concat([_scheduledTickers^, _activeTickers^]);
    _scheduledTickers := [];

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
    _activeTickers := _activeTickers^ |> List.map(f) |> _filterMap;
  };

  let _clear = (id: int, ()) => {
    _cancelledTickers := IntMap.add(id, true, _cancelledTickers^);
  };

  let interval = (f: callback, frequency: Time.t) => {
    let id = TickId.getUniqueId();

    let tf: tickFunction = {
      tickType: Interval,
      id,
      lastExecutionTime: ClockImpl.time(),
      frequency,
      f,
    };

    _scheduledTickers := [tf, ..._scheduledTickers^];
    _clear(id);
  };

  let timeout = (f, waitTime: Time.t) => {
    let id = TickId.getUniqueId();

    let f = _ => {
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
    _clear(id);
  };
};

module Default = Make(DefaultClock);
