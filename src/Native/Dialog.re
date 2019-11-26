[@noalloc] external alertSupported: unit => bool = "revery_alertSupported";

[@noalloc]
external openFiles:
  (~startDirectory: string=?, ~fileTypes: list(string)=?, string) =>
  option(array(string)) =
  "revery_alertOpenFiles";
