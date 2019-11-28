[@noalloc] external alertSupported: unit => bool = "revery_alertSupported";

external _openFiles:
  (
    ~startDirectory: option(string),
    ~fileTypes: option(array(string)),
    ~allowMultiple: bool,
    ~canChooseFiles: bool,
    ~canChooseDirectories: bool,
    ~showHidden: bool,
    ~buttonText: option(string),
    ~title: option(string),
    unit
  ) =>
  option(array(string)) =
  "revery_alertOpenFiles_bytecode" "revery_alertOpenFiles_native";

let openFiles =
    (
      ~startDirectory=?,
      ~fileTypes=?,
      ~allowMultiple=false,
      ~canChooseFiles=true,
      ~canChooseDirectories=false,
      ~showHidden=false,
      ~buttonText=?,
      ~title=?,
      (),
    ) =>
  _openFiles(
    ~startDirectory,
    ~fileTypes,
    ~allowMultiple,
    ~canChooseFiles,
    ~canChooseDirectories,
    ~showHidden,
    ~buttonText,
    ~title,
    (),
  );
