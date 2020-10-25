open Skia;

let emitPng = (path, surface) => {
  let image = Surface.makeImageSnapshot(surface);
  let data = Image.encodeToData(image);
  let dataString = Data.makeString(data);
  let fileOutputChannel = open_out_bin(path);
  output_string(fileOutputChannel, dataString);
  close_out(fileOutputChannel);
};

let makeSurface = (width, height) => {
  let imageInfo = ImageInfo.make(width, height, Rgba8888, Premul, None);
  Surface.makeRaster(imageInfo, 0, None);
};

let draw = canvas => {
  let fontManager = FontManager.makeDefault();
  let style = FontStyle.make(400, 5, Upright);
  let maybeTypeface =
    FontManager.matchFamilyStyle(fontManager, "Arial", style);
  let fill = Paint.make();

  Paint.setColor(fill, Color.makeArgb(0xFFl, 0xFFl, 0xFFl, 0xFFl));
  Paint.setTextSize(fill, 16.);
  Paint.setSubpixelText(fill, true);

  switch (maybeTypeface) {
  | None =>
    print_endline("Normal Arial not found. Ensure you have it available.")
  | Some(typeface) =>
    Paint.setTypeface(fill, typeface);
    Canvas.drawText(canvas, "Arial (System)", 10., 20., fill);
    let stream = Typeface.toStream(typeface);
    let length = Stream.getLength(stream);
    Printf.printf("Stream length: %d\n", length);
    let data = Data.makeFromStream(stream, length);
    let oc = open_out("Arial.ttf");
    print_endline("Writing Arial Normal to Arial.ttf...");
    Printf.fprintf(oc, "%s", Data.makeString(data));
    print_endline("Written!");
    close_out(oc);
  };

  let maybeTypeface =
    FontManager.matchFamilyStyle(fontManager, "Times New Roman", style);
  switch (maybeTypeface) {
  | None =>
    print_endline(
      "Normal Times New Roman not found. Ensure you have it available.",
    )
  | Some(typeface) =>
    Paint.setTypeface(fill, typeface);
    Canvas.drawText(canvas, "Times New Roman (System)", 10., 40., fill);
  };

  let maybeTypeface =
    FontManager.matchFamilyStyle(fontManager, "Consolas", style);
  switch (maybeTypeface) {
  | None =>
    print_endline(
      "Normal Consolas not found. Ensure your system has it available.",
    )
  | Some(typeface) =>
    Paint.setTypeface(fill, typeface);
    let metrics = FontMetrics.make();
    let _ret: float = Paint.getFontMetrics(fill, metrics, 1.0);
    print_endline("__Consolas__");
    print_endline(
      "-- Average character width: "
      ++ string_of_float(FontMetrics.getAvgCharacterWidth(metrics)),
    );
    print_endline(
      "-- Maximum character width: "
      ++ string_of_float(FontMetrics.getMaxCharacterWidth(metrics)),
    );
  };

  let filePath =
    Sys.getcwd()
    ++ "/packages/reason-skia/examples/skia-cli/FiraCode-Regular.ttf";
  print_endline("Loading font: " ++ filePath);
  let maybeTypeface = Typeface.makeFromFile(filePath, 0);
  switch (maybeTypeface) {
  | None => failwith("Unable to load font: " ++ filePath)
  | Some(typeface) =>
    Paint.setTypeface(fill, typeface);
    let metrics = FontMetrics.make();
    let _ret: float = Paint.getFontMetrics(fill, metrics, 1.0);
    print_endline("__Fira Code__");
    print_endline(
      "-- Average character width: "
      ++ string_of_float(FontMetrics.getAvgCharacterWidth(metrics)),
    );
    print_endline(
      "-- Maximum character width: "
      ++ string_of_float(FontMetrics.getMaxCharacterWidth(metrics)),
    );
  };

  let emoji = "😃";
  let char = Zed_utf8.get(emoji, 0);
  let maybeTypeface =
    FontManager.matchFamilyStyleCharacter(
      fontManager,
      "Arial",
      style,
      ["en_US"],
      char,
    );
  switch (maybeTypeface) {
  | Some(typeface) =>
    print_endline(
      "Found font for emoji: " ++ Typeface.getFamilyName(typeface),
    );
    Paint.setTypeface(fill, typeface);
    Canvas.drawText(canvas, emoji, 10., 60., fill);
  | None => print_endline("No emoji font found")
  };

  let japanese = "鬼";
  let char = Zed_utf8.get(japanese, 0);
  let maybeTypeface =
    FontManager.matchFamilyStyleCharacter(
      fontManager,
      "Arial",
      style,
      ["en_US"],
      char,
    );
  switch (maybeTypeface) {
  | Some(typeface) =>
    print_endline(
      "Found font for Japanese: " ++ Typeface.getFamilyName(typeface),
    );
    Paint.setTypeface(fill, typeface);
    Canvas.drawText(canvas, japanese, 10., 80., fill);
  | None => print_endline("No Japanese font found")
  };
};

let surface = makeSurface(640l, 480l) |> Option.get;
let canvas = Surface.getCanvas(surface);
draw(canvas);
emitPng("skia-font-manager-output.png", surface);
print_endline("Done!");
