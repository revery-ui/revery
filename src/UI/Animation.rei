open Revery_Core;

type t('a);

/**
 * Normalized time in the interval [0., 1.]
 */
type normalizedTime = pri float;

type state =
  | Delayed
  | Running(normalizedTime)
  | Complete(Time.t); // Elapsed time

let animate: Time.t => t(normalizedTime);
let delay: (Time.t, t('a)) => t('a);
let repeat: t('a) => t('a);
let alternatingRepeat: t('a) => t('a);
let ease: (Easing.t, t(normalizedTime)) => t(normalizedTime);
let tween: (float, float, t(normalizedTime)) => t(float);
let map: ('a => 'b, t('a)) => t('b);
let andThen: (t('a), ~next:t('a)) => t('a);
let zip: ((t('a), t('b))) => t(('a, 'b));
let apply: (Time.t, t('a)) => ('a, state);