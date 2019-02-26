type t = {
  top: int,
  left: int,
  width: int,
  height: int,
};

let create = (~top, ~left, ~width, ~height, ()) => {
  top,
  left,
  width,
  height,
};
