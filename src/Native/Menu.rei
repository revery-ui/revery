type t;

module Item: {
  type t;

  let hash: t => int;
  let equal: (t, t) => bool;
  let toString: t => string;

  let create: (~title: string, ~onClick: unit => unit) => t;
};

let getMenuBarHandle: unit => t;
let create: string => t;
let toString: t => string;
