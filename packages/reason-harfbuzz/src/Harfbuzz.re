type hb_shape = {
  glyphId: int,
  cluster: int,
};

module Internal = {
  type face;
  type feature = {
    tag: string,
    value: int,
    start: int,
    stop: int,
  };
  external hb_face_from_path: string => result(face, string) =
    "rehb_face_from_path";
  external hb_face_from_data: (string, int) => result(face, string) =
    "rehb_face_from_bytes";
  external hb_destroy_face: face => unit = "rehb_destroy_face";
  external hb_shape:
    (face, string, array(feature), int, int) => array(hb_shape) =
    "rehb_shape";

  // hb-version
  external hb_version_string_compiled: unit => string =
    "rehb_version_string_compiled";
  external hb_version_string_runtime: unit => string =
    "rehb_version_string_runtime";
};

type position = [ | `Start | `End | `Position(int)];
type feature = {
  tag: string,
  value: int,
  start: position,
  stop: position,
};

type hb_face = {face: Internal.face};

let positionToInt = position =>
  switch (position) {
  | `Position(n) => n
  | `Start => 0
  | `End => (-1)
  };

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

let hb_shape = (~features=[], ~start=`Start, ~stop=`End, {face}, str) => {
  let arr =
    features
    |> List.map(
         feat => {
           tag: feat.tag,
           value: feat.value,
           start: positionToInt(feat.start),
           stop: positionToInt(feat.stop),
         }: feature => Internal.feature,
       )
    |> Array.of_list;
  let startPosition = positionToInt(start);
  let length =
    switch (stop) {
    | `Position(n) => n - startPosition
    | `Start => 0
    | `End => (-1)
    };

  Internal.hb_shape(face, str, arr, startPosition, length);
};
let hb_new_face = str => hb_face_from_path(str);

let hb_face_from_data = bytes => {
  switch (Internal.hb_face_from_data(bytes, String.length(bytes))) {
  | Error(_) as e => e
  | Ok(face) =>
    let ret = {face: face};

    let finalise = ({face}) => Internal.hb_destroy_face(face);

    Gc.finalise(finalise, ret);
    Ok(ret);
  };
};

let hb_version_string_compiled = Internal.hb_version_string_compiled;
let hb_version_string_runtime = Internal.hb_version_string_runtime;
