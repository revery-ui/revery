[@noalloc] external alertSupported: unit => bool = "revery_alertSupported";

external _openFiles:
  (
    ~startDirectory: option(string),
    ~fileTypes: option(array(string)),
    ~allowMultipleFiles: bool,
    ~title: option(string),
    unit
  ) =>
  option(array(string)) =
  "revery_alertOpenFiles";

let openFiles =
    (~startDirectory=?, ~fileTypes=?, ~allowMultipleFiles=false, ~title=?, ()) =>
  _openFiles(~startDirectory, ~fileTypes, ~allowMultipleFiles, ~title, ());
