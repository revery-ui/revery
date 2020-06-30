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
    end': int,
  };
  external hb_new_face: string => result(face, string) = "rehb_new_face";
  external hb_destroy_face: face => unit = "rehb_destroy_face";
  external hb_shape: (face, string, array(feature)) => array(hb_shape) =
    "rehb_shape";
};

type position = [ | `Start | `End | `Position(int)];
type feature = {
  tag: string,
  value: int,
  start: position,
  end': position,
};

type hb_face = {face: Internal.face};

let positionToInt = position =>
  switch (position) {
  | `Position(n) => n
  | `Start => 0
  | `End => (-1)
  };

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

let hb_shape = (~features=[], {face}, str) => {
  let arr =
    features
    |> List.map(
         feat => {
           tag: feat.tag,
           value: feat.value,
           start: positionToInt(feat.start),
           end': positionToInt(feat.end'),
         }: feature => Internal.feature,
       )
    |> Array.of_list;
  Internal.hb_shape(face, str, arr);
};
