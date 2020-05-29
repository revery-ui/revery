open Revery_UI;

module Log = (val Revery_Core.Log.withNamespace("Revery.Hotreload"));

type obscure;

module type hotreload = {
  let text: string;
  let render: unit => React.element(React.reveryNode);
};

let table: Hashtbl.t(string, module hotreload) = Hashtbl.create(10);

let getPath = filename =>
  Filename.concat(
    Revery_Core.Environment.executingDirectory,
    Printf.sprintf("%s.cma", filename |> Filename.remove_extension),
  );

let setFile = (filename, _module) =>
  Hashtbl.replace(table, filename, _module);

let getFile = filename =>
  switch (Hashtbl.find_opt(table, filename)) {
  | Some(m) => m
  | None => failwith("Module has not been registered")
  };

let registerFile = filename => {
  let path = filename |> getPath |> Dynlink.adapt_filename;
  Dynlink.loadfile_private(path);
};

let start = () =>
  if (!Dynlink.is_native) {
    let dir = Revery_Core.Environment.getWorkingDirectory();
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
      | [@implicit_arity] Ok(path, events) => {
          let filename = Filename.basename(path);
          if (List.mem(`CHANGE, events) && Hashtbl.mem(table, filename)) {
            Log.infof(m => m("File %s was changed.", path));
            let command =
              switch (Sys.getenv_opt("REVERY_HOTRELOAD_COMMAND")) {
              | Some(c) => c
              | None => "dune build"
              };
            let result = Sys.command(command);
            if (result != 0) {
              Log.warnf(m =>
                m("Reload command %s failed with code %d", command, result)
              );
            } else {
              registerFile(filename);
            };
          };
        },
    );
  };

start();
