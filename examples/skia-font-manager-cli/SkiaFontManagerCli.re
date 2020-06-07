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
};

let surface = makeSurface(640l, 480l);
let canvas = Surface.getCanvas(surface);
draw(canvas);
emitPng("skia-font-manager-output.png", surface);
print_endline("Done!");
