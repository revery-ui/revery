open Reglfw.Glfw;

type t =
  | Seconds(float)
  | Milliseconds(float);

let of_float_seconds = (v: float) => Seconds(v);

let to_float_seconds = (v: t) =>
  switch (v) {
  | Seconds(x) => x
  | Milliseconds(x) => x /. 1000.
  };

let increment: (t, t) => t =
  (previousTime, duration) => {
    of_float_seconds(
      to_float_seconds(previousTime) +. to_float_seconds(duration),
    );
  };

let show = (v: t) => string_of_float(to_float_seconds(v)) ++ "s";

let getTime = () => of_float_seconds(glfwGetTime());
