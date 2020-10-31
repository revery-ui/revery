open Effect;
open Reducer;
open Tick;

let time = (~tickRate=Time.zero, ~name, ()) => {
  let%hook (time, setTime) = reducer(~initialState=Time.now(), t => t);
  let%hook () = tick(~tickRate, ~name, _dt => setTime(_t => Time.now()));

  time;
};

let timer = (~tickRate=Time.zero, ~active=true, ~name, ()) => {
  let%hook (time, setTime) = reducer(~initialState=Time.now(), t => t);
  let%hook startTime = Ref.ref(time);

  // HACK: We have to manually track disposal, too, to workaround a bug with
  // multiple timer hooks. This shouldn't be necessary - the `effect` hook
  // without `OnMountAndIf` should be handling disposal completely for us!

  // However, there is a bug when there are multiple timer hooks used -
  // if they are toggled on and off independently, we can get into a state
  // where a hanging ticker is left.

  // This is a very bad state because it will cause the UI to constantly
  // re-render, and be expensive for both CPU and battery life!

  // See the test case in:
  // test/UI/HooksTest.re that manifests this bug

  // Ideally, we can fix the underlying bug (perhaps in the effect hook),
  // and remove this hack while keeping all those tests green.

  // It's easy to hit when there are multiple springs associated with a component,
  // which each use an underlying timer.
  let%hook lastDispose = Ref.ref(None);

  let onTick = _dt => setTime(_t => Time.now());

  let%hook () =
    effect(OnMountAndIf((!=), active), () =>
      if (active) {
        startTime := Time.(now() - (time - startTime^));

        Option.iter(dispose => dispose(), lastDispose^);

        let stopInterval = Revery_Core.Tick.interval(~name, onTick, tickRate);
        lastDispose := Some(stopInterval);

        Some(stopInterval);
      } else {
        Option.iter(dispose => dispose(), lastDispose^);

        lastDispose := None;

        None;
      }
    );

  let reset = () => {
    startTime := time;
  };

  (Time.(time - startTime^), reset);
};
