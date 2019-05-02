module Color = Color_wrapper;
module Colors = Colors;
module Key = Key;
module MouseButton = MouseButton;
module MouseCursors = MouseCursors;

module Window = Window;
module App = App;
module Time = Time;
module Monitor = Monitor;

module Environment = Environment;

module Event = Event;
module Events = Events;

module Performance = Performance;
module UniqueId = UniqueId;

module TextWrapping = TextWrapping;
module TextOverflow = TextOverflow;

module WindowCreateOptions = WindowCreateOptions;

/*
 * Internally exposed modules, just for testing.
 */
module Internal = {
  module Tick = Tick;
};

module Tick = Tick.Default;

module Memoize = {
  type t('a, 'b) = 'a => 'b;

  let make = (f: t('a, 'b)): t('a, 'b) => {
    let tbl: Hashtbl.t('a, 'b) = Hashtbl.create(16);

    let ret = (arg: 'a) => {
      let cv = Hashtbl.find_opt(tbl, arg);
      switch (cv) {
      | Some(x) => x
      | None =>
        let r = f(arg);
        Hashtbl.add(tbl, arg, r);
        r;
      };
    };
    ret;
  };
};

module Lazy = {
  type t('a) = Memoize.t(unit, 'a);

  let make = (f: t('a)): t('a) => Memoize.make(f);
};
