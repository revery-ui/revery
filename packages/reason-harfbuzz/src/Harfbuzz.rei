type hb_face;

type hb_shape = {
  glyphId: int,
  cluster: int,
};

let hb_face_from_path: string => result(hb_face, string);
let hb_face_from_skia: Skia.Typeface.t => result(hb_face, string);

[@ocaml.deprecated "Deprecated in favor of hb_face_from_path"]
let hb_new_face: string => result(hb_face, string);

let hb_shape: (hb_face, string) => array(hb_shape);
