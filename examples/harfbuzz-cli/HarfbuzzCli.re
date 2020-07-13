open Harfbuzz;
open Skia;

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
  let fontManager = FontManager.makeDefault();
  let style = FontStyle.make(400, 5, Upright);
  let maybeTypeface =
    FontManager.matchFamilyStyle(fontManager, "Arial", style);
  let result = maybeTypeface |> Option.map(face => hb_face_from_skia(face));

  switch (result) {
  | Some(Error(msg)) => failwith(msg)
  | Some(Ok(font)) =>
    let show = ({glyphId, cluster}: hb_shape) =>
      Printf.sprintf("GlyphID: %d Cluster: %d", glyphId, cluster);

    let renderString = str => {
      let shapes = Harfbuzz.hb_shape(font, str);
      print_endline("-- " ++ str ++ " --");
      Array.iter(s => {print_endline("- SHAPE: " ++ show(s))}, shapes);
      print_endline("----");
    };

    renderString("abc");
    renderString("!=ajga");
    renderString("a==>b");
  | None => failwith("Font Arial not found!")
  };
};

run();
