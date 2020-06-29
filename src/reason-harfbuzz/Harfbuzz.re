type hb_shape = {
  glyphId: int,
  cluster: int,
};

module Internal = {
  type face;
  external hb_face_from_path: string => result(face, string) =
    "rehb_face_from_path";
  external hb_face_from_bytes: (string, int) => result(face, string) =
    "rehb_face_from_bytes";
  external hb_destroy_face: face => unit = "rehb_destroy_face";
  external hb_shape: (face, string) => array(hb_shape) = "rehb_shape";
};

type hb_face = {face: Internal.face};

let hb_face_from_path = str => {
  switch (Internal.hb_face_from_path(str)) {
  | Error(msg) => Error(msg)
  | Ok(face) =>
    let ret = {face: face};

    let finalise = ({face}) => Internal.hb_destroy_face(face);

    Gc.finalise(finalise, ret);
    Ok(ret);
  };
};

let hb_new_face = str => hb_face_from_path(str);

let hb_face_from_skia = sk_typeface => {
  let stream = Skia.Typeface.toStream(sk_typeface);
  let length = Skia.Stream.getLength(stream);
  let data = Skia.Data.makeFromStream(stream, length);
  let bytes = Skia.Data.makeString(data);

  switch (Internal.hb_face_from_bytes(bytes, String.length(bytes))) {
  | Error(msg) as e => e
  | Ok(face) =>
    let ret = {face: face};

    let finalise = ({face}) => Internal.hb_destroy_face(face);

    Gc.finalise(finalise, ret);
    Ok(ret);
  };
};

let hb_shape = ({face}, str) => Internal.hb_shape(face, str);
