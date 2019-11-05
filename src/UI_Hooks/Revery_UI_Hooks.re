include Animation;
include Effect;
include Reducer;
include Ref;
include State;
include Tick;

let time = (~tickRate=Time.Seconds(0.), hooks) => {
  let (time, setTime, hooks) =
    reducer(~initialState=Time.getTime(), t => t, hooks);
  let hooks = tick(~tickRate, _dt => setTime(_t => Time.getTime()), hooks);

  (time, hooks);
};
