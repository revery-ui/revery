open Harfbuzz;

Printexc.record_backtrace(true);

let isNative =
  switch (Sys.backend_type) {
  | Native => true
  | Bytecode => true
  | _ => false
  };

let getExecutingDirectory = () =>
  isNative ? Filename.dirname(Sys.argv[0]) ++ Filename.dir_sep : "";

let run = () => {
  let show = ({glyphId, cluster}: hb_shape) =>
    Printf.sprintf("GlyphID: %d Cluster: %d", glyphId, cluster);

  let renderString = (~features=[], font, str) => {
    let shapes = Harfbuzz.hb_shape(~features, font, str);
    print_endline("-- " ++ str ++ " --");
    Array.iter(s => {print_endline("- SHAPE: " ++ show(s))}, shapes);
    print_endline("----");
  };

  let compiledVersion = hb_version_string_compiled();
  let runtimeVersion = hb_version_string_runtime();
  print_endline(
    "** Harfbuzz CLI, compiled version: "
    ++ compiledVersion
    ++ ", runtime version: "
    ++ runtimeVersion
    ++ " **\n",
  );

  print_endline("__ Font Path: Roboto Regular __");
  let result = hb_face_from_path("test/collateral/FiraCode-Regular.ttf");
  let features = [
    Harfbuzz.{tag: "dlig", value: 1, start: `Start, stop: `End},
  ];
  switch (result) {
  | Error(msg) => failwith(msg)
  | Ok(font) =>
    renderString(font, "abc");
    renderString(font, "Harfbuzz");
    renderString(font, "ff");
    renderString(~features, font, "ff");
    renderString(font, "κόσμε");
  };
};

run();
