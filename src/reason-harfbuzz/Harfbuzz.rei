type hb_face;

type hb_shape = {
  glyphId: int,
  cluster: int,
};

let hb_new_face: string => result(hb_face, string);
let hb_shape: (hb_face, string) => array(hb_shape);
