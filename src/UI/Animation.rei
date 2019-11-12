open Revery_Core;

/**
 * Normalized time in the interval [0., 1.]
 */
module NormalizedTime: {
  type t = pri float;
  let fromFloat: float => t;
};

/**
 * `t('value)` is the type of an animated value where `'value` represents the type of the value.
 */
type t('value);

/**
 * `state` is the state of the animation.
 *
 *   `Delayed` means it has yet to start
 *   `Running` means it has started and not yet finished.
 *   `Complete(elapsed)` means it has completed in `elapsed` time.
 */
type state =
  | Delayed
  | Running
  | Complete(Time.t); // Elapsed time

/**
 * `const(value)` will always be completed and produce `value`
 *
 * Examples:
 *   const(3) |> valueAt(Time.zero) == 3,
 *   const(3) |> valueAt(Time.seconds(-42.)) == 3,
 *   const(3) |> valueAt(Time.seconds(42.)) == 3,
 *   const(3) |> stateAt(Time.seconds(-42.)) == Complete(Time.zero),
 *   const(3) |> stateAt(Time.seconds(42.)) == Complete(Time.zero),
 *   const("rhino") |> valueAt(Time.zero) == "rhino",
 */
let const: 'a => t('a);

/**
 * `animate(time)` maps the interval [Time.zero, time] to the interval [0., 1.]
 *
 * Applied with a time value < `Time.zero` will produce `(0., Delayed)`.
 * Applied with a time value > `time` will produce `(1., Complete(time))`.
 * Applied with a time value > `Time.zero` and < `time` will produce
 * `(value, Running(value))` where `value` is in the interval [0., 1.].
 *
 * Examples:
 *   (animate(Time.seconds(5.)) |> valueAt(Time.zero) :> float) == 0.0,
 *   (animate(Time.seconds(5.)) |> valueAt(Time.seconds(-42.)) :> float) == 0.0,
 *   (animate(Time.seconds(5.)) |> valueAt(Time.seconds(1.)) :> float) == 0.2,
 *   (animate(Time.seconds(5.)) |> valueAt(Time.seconds(42.)) :> float) == 1.0,
 *   animate(Time.seconds(5.)) |> stateAt(Time.zero) == Running,
 *   animate(Time.seconds(5.)) |> stateAt(Time.seconds(-42.)) == Delayed,
 *   animate(Time.seconds(5.)) |> stateAt(Time.seconds(1.)) == Running,
 *   animate(Time.seconds(5.)) |> stateAt(Time.seconds(42.)) == Complete(Time.seconds(5.)),
 */
let animate: Time.t => t(NormalizedTime.t);

/**
 * `delay(time)` will effectively extend the the reuslt of applying `Time.zero`
 *
 * Examples:
 *   (animate(Time.seconds(1.)) |> delay(Time.seconds(1.)) |> valueAt(Time.seconds(-2.)) :> float) == 0.0,
 *   (animate(Time.seconds(1.)) |> delay(Time.seconds(1.)) |> valueAt(Time.seconds(0.75)) :> float) == 0.0,
 *   (animate(Time.seconds(1.)) |> delay(Time.seconds(1.)) |> valueAt(Time.seconds(1.5)) :> float) == 0.5,
 *   (animate(Time.seconds(1.)) |> delay(Time.seconds(1.)) |> valueAt(Time.seconds(4.66)) :> float) == 1.0,
 *   animate(Time.seconds(1.)) |> delay(Time.seconds(1.)) |> stateAt(Time.seconds(-2.)) == Delayed,
 *   animate(Time.seconds(1.)) |> delay(Time.seconds(1.)) |> stateAt(Time.seconds(0.75)) == Delayed,
 *   animate(Time.seconds(1.)) |> delay(Time.seconds(1.)) |> stateAt(Time.seconds(1.5)) == Running,
 *   animate(Time.seconds(1.)) |> delay(Time.seconds(1.)) |> stateAt(Time.seconds(4.66)) == Complete(Time.seconds(2.)),
 */
let delay: (Time.t, t('a)) => t('a);

/**
 * `repeat` will repeat the animation ad infinitum
 *
 * Note that it will not repat "backwards", that is, in negative time.
 *
 * Examples:
 *   (animate(Time.seconds(1.)) |> repeat |> valueAt(Time.seconds(-2.)) :> float) == 0.0,
 *   (animate(Time.seconds(1.)) |> repeat |> valueAt(Time.seconds(0.75)) :> float) == 0.75,
 *   (animate(Time.seconds(1.)) |> repeat |> valueAt(Time.seconds(1.75)) :> float) == 0.75,
 *   (animate(Time.seconds(1.)) |> repeat |> valueAt(Time.seconds(3.66)) :> float) =~. 0.66,
 *   (animate(Time.seconds(1.)) |> repeat |> valueAt(Time.seconds(4.66)) :> float) =~. 0.66,
 *   animate(Time.seconds(1.)) |> repeat |> stateAt(Time.seconds(-2.)) == Delayed,
 *   animate(Time.seconds(1.)) |> repeat |> stateAt(Time.seconds(4.66)) == Running,
 */
let repeat: t('a) => t('a);

/**
 * `alternatingRepeat` will repeat the animation back and forth ad infinitum
 *
 * Note that it will not repat "backwards", that is, in negative time.
 *
 * Examples:
 *   (animate(Time.seconds(1.)) |> alternatingRepeat |> valueAt(Time.seconds(-2.)) :> float) == 0.0,
 *   (animate(Time.seconds(1.)) |> alternatingRepeat |> valueAt(Time.seconds(0.75)) :> float) == 0.75,
 *   (animate(Time.seconds(1.)) |> alternatingRepeat |> valueAt(Time.seconds(1.75)) :> float) == 0.25,
 *   (animate(Time.seconds(1.)) |> alternatingRepeat |> valueAt(Time.seconds(3.66)) :> float) =~. 0.34,
 *   (animate(Time.seconds(1.)) |> alternatingRepeat |> valueAt(Time.seconds(4.66)) :> float) =~. 0.66,
 *   animate(Time.seconds(1.)) |> alternatingRepeat |> stateAt(Time.seconds(-2.)) == Delayed,
 *   animate(Time.seconds(1.)) |> alternatingRepeat |> stateAt(Time.seconds(4.66)) == Running,
 */
let alternatingRepeat: t('a) => t('a);

/**
 * `ease(start, end)` will transform the animated value according to the given easing function.
 *
 * Note that easing only works in normalized time space, and therefore requires
 * an animation in normalized time even if the easing functions themselves don't.
 *
 * Examples:
 *   (animate(Time.seconds(1.)) |> ease(Easing.quadratic) |> valueAt(Time.seconds(-2.)) :> float) == 0.0,
 *   (animate(Time.seconds(1.)) |> ease(Easing.quadratic) |> valueAt(Time.seconds(0.25)) :> float) == 0.0625,
 *   (animate(Time.seconds(1.)) |> ease(Easing.quadratic) |> valueAt(Time.seconds(0.5)) :> float) == 0.25,
 *   (animate(Time.seconds(1.)) |> ease(Easing.quadratic) |> valueAt(Time.seconds(0.75)) :> float) =~. 0.5625,
 *   (animate(Time.seconds(1.)) |> ease(Easing.quadratic) |> valueAt(Time.seconds(4.66)) :> float) =~. 1.0,
 */
let ease: (Easing.t, t(NormalizedTime.t)) => t(NormalizedTime.t);

/**
 * `tween(start, end)` maps normalized time to the interval [start, end]
 *
 * Examples:
 *   animate(Time.seconds(1.)) |> tween(2., 5.) |> valueAt(Time.zero) == 2.0,
 *   animate(Time.seconds(1.)) |> tween(2., 5.) |> valueAt(Time.seconds(0.5)) == 3.5,
 *   animate(Time.seconds(1.)) |> tween(2., 5.) |> valueAt(Time.seconds(-42.)) == 2.0,
 *   animate(Time.seconds(1.)) |> tween(2., 5.) |> valueAt(Time.seconds(42.)) == 5.0,
 */
let tween: (float, float, t(NormalizedTime.t)) => t(float);

/**
 * `map(f)` maps the animated value according to the given function `f`
 *
 * Examples:
 *   animate(Time.seconds(1.)) |> map(n => string_of_float(n: NormalizedTime.t :> float)) |> valueAt(Time.seconds(0.33)) == "0.33",
 */
let map: ('a => 'b, t('a)) => t('b);

/**
 * `andThen(a, ~next:b)` runs `a` and `b` in sequence.
 *
 * Examples:
 *   (animate(Time.seconds(1.)) |> andThen(~next=animate(Time.seconds(2.))) |> valueAt(Time.seconds(1.)) :> float) == 1.0,
 *   (animate(Time.seconds(1.)) |> andThen(~next=animate(Time.seconds(2.))) |> valueAt(Time.seconds(2.)) :> float) == 0.5,
 */
let andThen: (t('a), ~next: t('a)) => t('a);

/**
 * `zip((a, b))` runs `a` and `b` in parallel and returns their values as a tuple.
 *
 * Examples:
 *   (zip((animate(Time.seconds(1.)), animate(Time.seconds(2.)))) |> valueAt(Time.seconds(1.)) |> fst :> float) == 1.0,
 *   (zip((animate(Time.seconds(1.)), animate(Time.seconds(2.)))) |> valueAt(Time.seconds(1.)) |> snd :> float) == 0.5,
 */
let zip: ((t('a), t('b))) => t(('a, 'b));

/**
 * `apply(time)` returns the vale and state of the animation at the given `time`
 *
 * Examples:
 *   (animate(Time.seconds(1.)) |> apply(Time.seconds(0.33)) |> fst :> float) == 0.33,
 *   animate(Time.seconds(1.)) |> apply(Time.seconds(0.33)) |> snd == Running,
 */
let apply: (Time.t, t('a)) => ('a, state);

/**
 * `valueAt(time)` returns the value of the animation at the given `time`
 *
 * Examples:
 *   (animate(Time.seconds(1.)) |> valueAt(Time.seconds(0.33)) :> float) == 0.33,
 */
let valueAt: (Time.t, t('a)) => 'a;

/**
 * `stateAt(time)` returns the state of the animation at the given `time`
 *
 * Examples:
 *   animate(Time.seconds(1.)) |> stateAt(Time.seconds(0.33)) == Running,
 */
let stateAt: (Time.t, t('a)) => state;
