open Revery_Core;

type t('a);

/**
 * Normalized time in the interval [0., 1.]
 */
module NormalizedTime: {
  type t = pri float;
  let fromFloat: float => t;
};

type t('a);

type state =
  | Delayed
  | Running
  | Complete(Time.t); // Elapsed time

let const: 'a => t('a);
let animate: Time.t => t(normalizedTime);
let delay: (Time.t, t('a)) => t('a);
let repeat: t('a) => t('a);
let alternatingRepeat: t('a) => t('a);
let ease: (Easing.t, t(normalizedTime)) => t(normalizedTime);
let tween: (float, float, t(normalizedTime)) => t(float);
let map: ('a => 'b, t('a)) => t('b);
let andThen: (t('a), ~next: t('a)) => t('a);
let zip: ((t('a), t('b))) => t(('a, 'b));

let apply: (Time.t, t('a)) => ('a, state);
let valueAt: (Time.t, t('a)) => 'a;
let stateAt: (Time.t, t('a)) => state;