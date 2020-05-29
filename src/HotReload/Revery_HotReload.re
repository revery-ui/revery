open Revery_UI;

module Log = (val Revery_Core.Log.withNamespace("Revery.Hotreload"));

module type hotreload = {
  let text: string;
  let render: unit => React.element(React.reveryNode);
};

let moduleTable: Hashtbl.t(string, module hotreload) = Hashtbl.create(10);

type callback = (module hotreload) => unit;
let callbackTable: Hashtbl.t(string, callback) = Hashtbl.create(10);

let getPath = filename =>
  Filename.concat(
    Revery_Core.Environment.executingDirectory,
    Printf.sprintf("%s.cma", filename),
  );

let register = (filename, _module) => {
  Hashtbl.replace(moduleTable, filename, _module);
  Hashtbl.find_all(callbackTable, filename) |> List.iter(c => c(_module));
};

let getModule = filename =>
  switch (Hashtbl.find_opt(moduleTable, filename)) {
  | Some(m) => m
  | None => failwith("Module has not been registered")
  };

let watch = (filename, callback) => {
  let path = filename |> getPath |> Dynlink.adapt_filename;
  Hashtbl.add(callbackTable, filename, callback);
  Dynlink.loadfile_private(path);
};

let start = () =>
  if (!Dynlink.is_native) {
    let dir = Revery_Core.Environment.executingDirectory;
    let watcher = Luv.FS_event.init() |> Result.get_ok;

    Luv.FS_event.start(
      ~recursive=true,
      watcher,
      dir,
      fun
      | Error(e) => {
          Log.warnf(m =>
            m("Could not watch %s: %s", dir, Luv.Error.strerror(e))
          );
          ignore(Luv.FS_event.stop(watcher));
          Luv.Handle.close(watcher, ignore);
        }
      | Ok((path, _events)) => {
          let _filename = Filename.basename(path);
          Log.infof(m => m("File %s was changed.", path));
        },
    );
  };

start();
