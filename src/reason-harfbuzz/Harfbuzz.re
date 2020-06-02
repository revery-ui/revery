type hb_shape = {
  glyphId: int,
  cluster: int,
};

module Internal = {
  type face;
  external hb_new_face: string => result(face, string) = "rehb_new_face";
  external hb_destroy_face: face => unit = "rehb_destroy_face";
  external hb_shape: (face, string) => array(hb_shape) = "rehb_shape";
};

type hb_face = {face: Internal.face};

let hb_new_face = str => {
  switch (Internal.hb_new_face(str)) {
  | Error(msg) => Error(msg)
  | Ok(face) =>
    let ret = {face: face};

    let finalise = ({face}) => Internal.hb_destroy_face(face);

    Gc.finalise(finalise, ret);
    Ok(ret);
  };
};

let hb_shape = ({face}, str) => Internal.hb_shape(face, str);
