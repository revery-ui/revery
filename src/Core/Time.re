open Reglfw.Glfw;

type t = 
| Seconds(float)
| Milliseconds(float);

let of_float_seconds = (v:float) => {
    Seconds(v);
};

let to_float_seconds = (v: t) => {
    switch (v) {
    | Seconds(x) => x
    | Milliseconds(x) => x /. 1000.;
    }
}

let show = (v: t) => {
    string_of_float(to_float_seconds(v)) ++ "s";
};

let getTime = () => of_float_seconds(glfwGetTime());
