[@noalloc] external alertSupported: unit => bool = "revery_alertSupported";

external openFiles:
  (
    ~startDirectory: string=?,
    ~fileTypes: array(string)=?,
    ~allowMultipleFiles: bool=?,
    ~title: string=?,
    unit
  ) =>
  option(array(string)) =
  "revery_alertOpenFiles";
