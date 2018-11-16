let module Color = Color;
let module Colors = Colors;

let module Window = Window;
let module App = App;
let module Time = Time;
let module Monitor = Monitor;

let module Event = Reactify.Event;

module Performance {
    type performanceFunction =  unit => unit;

    let bench = (name, f) => {
        let startTime = Unix.gettimeofday();
        f();
        let endTime = Unix.gettimeofday();
        print_endline("[PERF: " ++ name ++ "]: " ++ string_of_float((endTime -. startTime) /. 1000.) ++ "ms");
    };
}

module Lazy {
    type innerFunc('a) = unit => 'a;

    let make = (f: innerFunc('a)): innerFunc('a) => {
        let v: ref(option('a)) = ref(None);

        let ret = () => {
            switch(v^) {
            | Some(x) => x
            | None => {
                let out = f();
                v := Some(out);
                out
                }
            };
        };
        ret;
    };
}
