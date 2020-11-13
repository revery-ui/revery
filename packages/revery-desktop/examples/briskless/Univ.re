module Witness = {
  type id(_) = ..;

  module type S = {
    type t;
    type id(_) +=
      | Id: id(t);
  };

  type t('a) = (module S with type t = 'a);

  let create = (type u, ()) => {
    module W = {
      type t = u;
      type id(_) +=
        | Id: id(t);
    };
    ((module W): (module S with type t = u));
  };
};

type t =
  | Pack(Witness.id('a), 'a): t;

let pack = (type u, module Witness: Witness.S with type t = u, value) =>
  Pack(Witness.Id, value);

let unpack =
    (type u, module Witness: Witness.S with type t = u, Pack(id, value))
    : option(u) =>
  switch (id) {
  | Witness.Id => Some(value)
  | _ => None
  };
