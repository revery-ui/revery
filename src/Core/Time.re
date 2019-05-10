open Reglfw.Glfw;

type t =
  | Seconds(float)
  | Milliseconds(float);

/**
@deprecated Replaced by [toSeconds]
*/
[@deprecated]
let of_float_seconds = (v: float) => Seconds(v);

/**
@deprecated Replaced by [ofSeconds]
*/
[@deprecated]
let to_float_seconds = (v: t) =>
  switch (v) {
  | Seconds(x) => x
  | Milliseconds(x) => x /. 1000.
  };

/**
  [ofSeconds(time)] converts the time [t] to a float value representing the time in seconds.
*/
let ofSeconds = of_float_seconds;

/**
  [ofSeconds(seconds)] converts the value seconds to a [t] representing the time in seconds.
*/
let toSeconds = to_float_seconds;

let increment: (t, t) => t =
  (previousTime, duration) => {
    of_float_seconds(
      to_float_seconds(previousTime) +. to_float_seconds(duration),
    );
  };

let show = (v: t) => string_of_float(to_float_seconds(v)) ++ "s";

/**
    [getTime()] returns the current system time [t]
*/
let getTime = () => of_float_seconds(glfwGetTime());
