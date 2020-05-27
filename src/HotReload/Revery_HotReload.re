open Revery_UI;

module Log = (val Revery_Core.Log.withNamespace("Revery.Hotreload"));

module type hotreload = {let render: unit => React.element(React.reveryNode);
};

let table: Hashtbl.t(string, module hotreload) = Hashtbl.create(10);

let setModule = (path, _module) => Hashtbl.replace(table, path, _module);

let getModule = path =>
  switch (Hashtbl.find_opt(table, path)) {
  | Some(x) => x
  | None => failwith("Module has not been registered")
  };

let registerFile = filename => {
  let path =
    Filename.concat(
      Revery_Core.Environment.executingDirectory,
      Printf.sprintf("%s.cmxs", filename),
    );
  Dynlink.loadfile_private(path);
};
