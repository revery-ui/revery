open Revery_Font;

type t = {
  family: Family.t,
  weight: Weight.t,
  italicized: bool,
  monospaced: bool,
};

let default = {
  family: Family.default,
  weight: Weight.Normal,
  italicized: false,
  monospaced: false,
};

let make =
    (
      ~family=Family.default,
      ~weight=Weight.Normal,
      ~italicized=false,
      ~monospaced=false,
      (),
    ) => {
  family,
  weight,
  italicized,
  monospaced,
};
