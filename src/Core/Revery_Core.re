module Color = Color;
module Colors = Colors;

module Window = Window;
module App = App;
module Time = Time;
module Monitor = Monitor;

module Event = Reactify.Event;

module Performance = Performance;

module Lazy = {
  type innerFunc('a) = unit => 'a;

  let make = (f: innerFunc('a)): innerFunc('a) => {
    let v: ref(option('a)) = ref(None);

    let ret = () =>
      switch (v^) {
      | Some(x) => x
      | None =>
        let out = f();
        v := Some(out);
        out;
      };
    ret;
  };
};
