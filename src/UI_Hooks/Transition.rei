open Revery_UI.Animated;
open Revery_UI.React.Hooks;

let transition:
  (
    float,
    ~delay: Revery_Core.Time.t=?,
    ~duration: Revery_Core.Time.t=?,
    t((ref(animation), Reducer.t(int), Effect.t(Effect.onMount)) => 'a, 'b)
  ) =>
  (float, float => unit, t('a, 'b));
