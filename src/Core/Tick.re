module type Clock = {let time: unit => Time.t;};

module DefaultClock = {
  let time = () => Time.getTime();
};

type callback = Time.t => unit;
type dispose = unit => unit;

module Make = (ClockImpl: Clock) => {
  module TickId =
    UniqueId.Make({});

  type tickFunction = {
    id: int,
    lastExecutionTime: Time.t,
    frequency: Time.t,
    f: callback,
  };

  let _activeTickers: ref(list(tickFunction)) = ref([]);

  let pump = () => {
    let currentTime = Time.to_float_seconds(ClockImpl.time());

    let f = (tf: tickFunction) => {
      let lastTime = Time.to_float_seconds(tf.lastExecutionTime);
      let frequency = Time.to_float_seconds(tf.frequency);
      let nextTime = lastTime +. frequency;

      if (nextTime <= currentTime) {
        let elapsedTime = Time.of_float_seconds(currentTime -. lastTime);
        tf.f(elapsedTime);
        {...tf, lastExecutionTime: Time.of_float_seconds(currentTime)};
      } else {
        tf;
      };
    };
    _activeTickers := List.map(f, _activeTickers^);
  };

  let _clear = (id: int, ()) => {
    _activeTickers := List.filter(v => v.id !== id, _activeTickers^);
  };

  let interval = (f: callback, frequency: Time.t) => {
    let id = TickId.getUniqueId();

    let tf: tickFunction = {
      id,
      lastExecutionTime: ClockImpl.time(),
      frequency,
      f,
    };

    _activeTickers := List.append([tf], _activeTickers^);
    _clear(id);
  };

  let timeout = (f, waitTime: Time.t) => {
    let id = TickId.getUniqueId();

    let f = _ => {
      f();
      _clear(id, ());
    };

    let tf: tickFunction = {
      id,
      lastExecutionTime: ClockImpl.time(),
      frequency: waitTime,
      f,
    };

    _activeTickers := List.append([tf], _activeTickers^);
    _clear(id);
  };
};

module Default = Make(DefaultClock);
