module Witness: {
  type t(_);
  let create: unit => t(_);
};

type t;

let pack: (Witness.t('a), 'a) => t;
let unpack: (Witness.t('a), t) => option('a);
