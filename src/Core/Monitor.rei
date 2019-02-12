type t;

let getPrimaryMonitor: unit => t;

type size = {
  width: int,
  height: int,
};

let getSize: t => size;

let getScaleFactor: unit => int;
