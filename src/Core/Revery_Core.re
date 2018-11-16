let module Color = Color;
let module Colors = Colors;

let module Window = Window;
let module App = App;
let module Time = Time;
let module Monitor = Monitor;

let module Event = Reactify.Event;

module Performance = Performance;

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
