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
  let skiaFaceToHarfbuzzFace = skiaFace => {
    let stream = Skia.Typeface.toStream(skiaFace);
    let length = Skia.Stream.getLength(stream);
    let data = Skia.Data.makeFromStream(stream, length);
    let bytes = Skia.Data.makeString(data);

    Harfbuzz.hb_face_from_data(bytes);
  };

  print_endline("__ Font Discovery: Arial __");
  let maybeTypeface =
    FontManager.matchFamilyStyle(fontManager, "Arial", style);
  let result = maybeTypeface |> Option.map(skiaFaceToHarfbuzzFace);
  switch (result) {
  | Some(Error(msg)) => failwith(msg)
  | Some(Ok(font)) =>
    renderString(font, "abc");
    renderString(font, "!=ajga");
    renderString(font, "a==>b");
  | None => failwith("Font Arial not found!")
  };

  print_endline("__ Font Path: Roboto Regular __");
  let result = hb_face_from_path("examples/Roboto-Regular.ttf");
  let features = [
    Harfbuzz.{tag: "dlig", value: 1, start: `Start, stop: `End},
  ];
  switch (result) {
  | Error(msg) => failwith(msg)
  | Ok(font) =>
    renderString(font, "ff");
    renderString(~features, font, "ff");
  };
};

run();
