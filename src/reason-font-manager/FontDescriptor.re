type raw = {
  path: string,
  postscriptName: string,
  family: string,
  weight: int,
  width: int,
  italic: bool,
  monospace: bool,
};

type t = {
  path: string,
  postscriptName: string,
  family: string,
  weight: FontWeight.t,
  width: FontWidth.t,
  italic: bool,
  monospace: bool,
};

let ofRaw = (v: raw) => {
  path: v.path,
  postscriptName: v.postscriptName,
  family: v.family,
  weight: FontWeight.ofInt(v.weight),
  width: FontWidth.ofInt(v.width),
  italic: v.italic,
  monospace: v.monospace,
};

let show = (v: t) => {
  "FontDescriptor:\n"
  ++ " - Path: "
  ++ v.path
  ++ "\n"
  ++ " - Family: "
  ++ v.family
  ++ "\n"
  ++ " - Weight: "
  ++ FontWeight.show(v.weight)
  ++ "\n"
  ++ " - Width: "
  ++ FontWidth.show(v.width)
  ++ "\n"
  ++ " - Monospace: "
  ++ string_of_bool(v.monospace)
  ++ "\n";
};
