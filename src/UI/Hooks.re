/* Hooks specific to Revery */
open Animated;

let animation = (v: animationValue, opts: animationOptions, slots) => {
  let (playing, setPlaying, slots) = React.Hooks.ref(true, slots);
  let (currentV, _set, slots) = React.Hooks.state(v, slots);
  let (frame, pump, slots) = React.Hooks.state(0, slots);

  let slots =
    React.Hooks.effect(
      OnMount,
      () => {
        let complete = () => setPlaying(false);

        let {stop, _} = tween(v, opts) |> start(~complete);

        Some(() => stop());
      },
      slots,
    );

  let slots =
    React.Hooks.effect(
      Always,
      () => {
        /*
           If the animation is active, force a state change
           so that the component gets re-rendered
         */
        if (playing) {
          pump(frame + 1);
        };

        None;
      },
      slots,
    );

  (currentV.current, slots);
};
