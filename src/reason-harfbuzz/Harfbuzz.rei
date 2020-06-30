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
  end': position,
};

let hb_new_face: string => result(hb_face, string);
let hb_shape:
  (~features: list(feature)=?, hb_face, string) => array(hb_shape);
