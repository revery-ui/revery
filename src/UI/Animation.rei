open Revery_Core;

type t('a);

type state =
  | Delayed
  | Running(float) // Elapsed time, normalized, i.e. in the range [0., 1.]
  | Complete(Time.t); // Elapsed time

type normalizedTime = pri float;

let animate: Time.t => t(normalizedTime);
let delay: (Time.t, t('a)) => t('a);
let repeat: t('a) => t('a);
let ease: (Easing.t, t(normalizedTime)) => t(normalizedTime);
let tween: (float, float, t(normalizedTime)) => t(float);
let map: ('a => 'b, t('a)) => t('b);
let andThen: (t('a), ~next:t('a)) => t('a);
let apply: (Time.t, t('a)) => ('a, state);