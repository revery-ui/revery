open Revery_UI;

module Log = (val Revery_Core.Log.withNamespace("Revery.Hotreload"));

type obscure;

module type hotreload = {
  let text: string;
  let render: unit => React.element(React.reveryNode);
};

let table: Hashtbl.t(string, (module hotreload, float)) =
  Hashtbl.create(10);

let getPath = filename =>
  Filename.concat(
    Revery_Core.Environment.executingDirectory,
    Printf.sprintf("%s.cma", filename),
  );

let setModule = (filename, _module) => {
  let stats = Unix.stat(getPath(filename));
  Hashtbl.replace(table, filename, (_module, stats.st_mtime));
};

let getModule = filename =>
  switch (Hashtbl.find_opt(table, filename)) {
  | Some((m, _)) => m
  | None => failwith("Module has not been registered")
  };

let registerFile = filename => {
  let path = filename |> getPath |> Dynlink.adapt_filename;
  Dynlink.loadfile_private(path);
};

let thread =
  if (!Dynlink.is_native) {
    Thread.create(
      () =>
        while (true) {
          Hashtbl.to_seq(table)
          |> Seq.iter(((filename, (_, time))) =>
               if (time != Unix.stat(getPath(filename)).st_mtime) {
                 registerFile(filename);
               }
             );
          Unix.sleepf(2.0);
        },
      (),
    )
    |> ignore;
  };
