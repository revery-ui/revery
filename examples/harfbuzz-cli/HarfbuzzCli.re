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
  let fontPath = getExecutingDirectory() ++ "Roboto-Regular.ttf";
  let result = Harfbuzz.hb_new_face(fontPath);

  switch (result) {
  | Error(msg) => failwith(msg)
  | Ok(font) =>
    let show = ({glyphId, cluster}: hb_shape) =>
      Printf.sprintf("GlyphID: %d Cluster: %d", glyphId, cluster);

    let renderString = str => {
      let shapes = Harfbuzz.hb_shape(font, str);
      Array.iter(s => {print_endline("- SHAPE: " ++ show(s))}, shapes);
    };

    renderString("abc");
    renderString("!=ajga");
    renderString("a==>b");
  };
};

run();
