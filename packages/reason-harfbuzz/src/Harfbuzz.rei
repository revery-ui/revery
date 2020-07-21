type hb_face;

type hb_shape = {
  glyphId: int,
  cluster: int,
};

type position = [ | `Start | `End | `Position(int)];
type feature = {
  tag: string,
  value: int,
  start: position,
  stop: position,
};

let hb_face_from_path: string => result(hb_face, string);
let hb_face_from_data: string => result(hb_face, string);

[@ocaml.deprecated "Deprecated in favor of hb_face_from_path"]
let hb_new_face: string => result(hb_face, string);

let hb_shape:
  (
    ~features: list(feature)=?,
    ~start: position=?,
    ~stop: position=?,
    hb_face,
    string
  ) =>
  array(hb_shape);

let hb_version_string_compiled: unit => string;
let hb_version_string_runtime: unit => string;
