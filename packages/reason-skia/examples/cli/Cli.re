open Skia;

let getExecutingDirectory = () =>
  switch (Sys.backend_type) {
  | Native
  | Bytecode => Filename.dirname(Sys.argv[0]) ++ Filename.dir_sep
  | _ => ""
  };

let makeSurface = (width, height) => {
  let imageInfo = ImageInfo.make(width, height, Rgba8888, Premul, None);
  Surface.makeRaster(imageInfo, 0, None);
};

let emitPng = (path, surface) => {
  let image = Surface.makeImageSnapshot(surface);
  let data = Image.encodeToData(image);
  let dataString = Data.makeString(data);
  let fileOutputChannel = open_out_bin(path);
  output_string(fileOutputChannel, dataString);
  close_out(fileOutputChannel);
};

let draw = canvas => {
  let fill = Paint.make();
  Paint.setColor(fill, Color.makeArgb(0xFFl, 0x00l, 0x00l, 0xFFl));
  Canvas.drawPaint(canvas, fill);

  Paint.setColor(fill, Color.makeArgb(0xFFl, 0x00l, 0xFFl, 0xFFl));
  let rect = Rect.makeLtrb(100., 100., 540., 380.);
  Canvas.drawRect(canvas, rect, fill);

  let stroke = Paint.make();
  Paint.setColor(stroke, Color.makeArgb(0xFFl, 0xFFl, 0x00l, 0x00l));
  Paint.setAntiAlias(stroke, true);
  Paint.setStyle(stroke, Stroke);
  Paint.setStrokeWidth(stroke, 5.);

  let path = Path.make();
  Path.moveTo(path, 50., 50.);
  Path.lineTo(path, 590., 50.);
  Path.cubicTo(path, -490., 50., 1130., 430., 50., 430.);
  Path.lineTo(path, 590., 430.);
  Path.close(path);
  Path.addCircle(path, 100., 200., ~radius=75., ());

  let roundRect = Rect.makeLtrb(300., 400., 240., 280.);
  Path.addRoundRect(path, roundRect, 25., 25., ());
  Path.addCircle(path, 100., 300., ~radius=100., ~direction=`cw, ());
  Canvas.drawPath(canvas, path, stroke);

  Paint.setColor(fill, Color.makeArgb(0xCCl, 0x00l, 0xFFl, 0x00l));
  Paint.setImageFilter(
    fill,
    ImageFilter.makeDropShadow(
      10.,
      10.,
      3.,
      3.,
      Color.makeArgb(0xAAl, 0x00l, 0x00l, 0x00l),
      DrawShadowAndForeground,
      None,
      None,
    ),
  );
  let rect2 = Rect.makeLtrb(120., 120., 520., 360.);
  Canvas.drawOval(canvas, rect2, fill);

  let fill3 = Paint.make();
  Paint.setColor(fill3, Color.makeArgb(0xFFl, 0xFFl, 0xFFl, 0xFFl));
  Paint.setTextSize(fill3, 30.);

  let nonExistentTypeface = Typeface.makeFromFile("non-existent-font.ttf", 0);
  assert(nonExistentTypeface == None);

  // Draw text
  let filePath = getExecutingDirectory() ++ "Orbitron Medium.ttf";
  print_endline("Loading font: " ++ filePath);
  let maybeTypeface = Typeface.makeFromFile(filePath, 0);
  switch (maybeTypeface) {
  | None => failwith("Unable to load font: " ++ filePath)
  | Some(typeFace) =>
    print_endline(__LOC__ ++ ": we will set.");
    Paint.setTypeface(fill3, typeFace);
    print_endline(__LOC__ ++ ": setTypeface is OK.");
    Canvas.drawText(canvas, "Hello, world!", 30., 30., fill3);
    let metrics = FontMetrics.make();
    let _ret: float = Paint.getFontMetrics(fill3, metrics, 1.0);

    print_endline(
      "-- Top: " ++ string_of_float(FontMetrics.getTop(metrics)),
    );
    print_endline(
      "-- Bottom: " ++ string_of_float(FontMetrics.getBottom(metrics)),
    );
    print_endline(
      "-- Underline position: "
      ++ string_of_float(FontMetrics.getUnderlinePosition(metrics)),
    );
    print_endline(
      "-- Underline thickness: "
      ++ string_of_float(FontMetrics.getUnderlineThickness(metrics)),
    );
    print_endline("-- Ret: " ++ string_of_float(_ret));

    print_endline(__LOC__ ++ ": We return.");

    // Measure
    let measurement = Paint.measureText(fill3, "Hello, world!", None);
    print_endline("Measured text: " ++ string_of_float(measurement));
    Paint.setTextSize(fill3, 50.);
    let largeMeasurement = Paint.measureText(fill3, "Hello, world!", None);
    print_endline(
      "Large measured text: " ++ string_of_float(largeMeasurement),
    );
  };

  // Validate loading a non-existent file returns None, but doesn't crash
  let nonExistentData = Data.makeFromFileName("file-that-does-not-exist.png");
  switch (nonExistentData) {
  | Some(_) => failwith("Somehow we loaded a non-existent file...")
  | None => print_endline("Got None for file that doesn't exist.")
  };

  // Load and draw image
  let imgPath = getExecutingDirectory() ++ "uv.png";
  print_endline("Loading image: " ++ imgPath);
  let maybeImgData = Data.makeFromFileName(imgPath);
  let maybeImg =
    switch (maybeImgData) {
    | Some(imgData) =>
      let strLen = String.length(Data.makeString(imgData));
      print_endline("Bytes loaded: " ++ string_of_int(strLen));
      let ret = Image.makeFromEncoded(imgData, None);
      print_endline("Got image!");
      ret;
    | None => failwith("Unable to load img: " ++ imgPath)
    };

  switch (maybeImg) {
  | None => failwith("Unable to load image: uv.png")
  | Some(img) =>
    let imgFill = Paint.make();
    Paint.setAlpha(imgFill, 0.0);
    Canvas.drawImage(canvas, img, 250., 250., Some(imgFill));

    let sourceRect = Rect.makeLtrb(0., 0., 128., 128.);
    let destRect = Rect.makeLtrb(200., 200., 264., 264.);
    Canvas.drawImageRect(
      canvas,
      img,
      Some(sourceRect),
      destRect,
      Some(imgFill),
    );
  };

  // Draw text w/ ligature
  let filePath = getExecutingDirectory() ++ "FiraCode-Regular.ttf";
  print_endline("Loading font: " ++ filePath);
  let maybeTypeface = Typeface.makeFromFile(filePath, 0);
  switch (maybeTypeface) {
  | None => failwith("Unable to load font: " ++ filePath)
  | Some(typeFace) =>
    let fill = Paint.make();
    Paint.setColor(fill, Color.makeArgb(0xFFl, 0xFFl, 0xFFl, 0xFFl));
    Paint.setTextSize(fill, 30.);
    Paint.setTypeface(fill, typeFace);
    Paint.setSubpixelText(fill, true);
    Paint.setTextEncoding(fill, GlyphId);

    let glyphsToString = glyphs => {
      let len = List.length(glyphs);
      let bytes = Bytes.create(len * 2);

      let rec loop = (glyphs, idx) => {
        switch (glyphs) {
        | [hd, ...tail] =>
          let lowerBit = hd land 255;
          let highBit = (hd land 255 lsl 8) lsr 8;
          Bytes.set(bytes, idx * 2 + 0, Char.chr(lowerBit));
          Bytes.set(bytes, idx * 2 + 1, Char.chr(highBit));
          loop(tail, idx + 1);
        | [] => ()
        };
      };

      loop(glyphs, 0);

      Bytes.to_string(bytes);
    };

    // For FiraCode, this is a==>b
    let str = glyphsToString([136, 1624, 1624, 1495, 148]);
    Canvas.drawText(canvas, str, 50., 100., fill);
  };

  let fill = Paint.make();
  Paint.setColor(fill, Color.makeArgb(0xFFl, 0xFFl, 0x00l, 0xFFl));
  Canvas.drawRectLtwh(canvas, 50., 75., 100., 200., fill);

  let fill = Paint.make();
  Paint.setColor(fill, Color.makeArgb(0xFFl, 0xFFl, 0x00l, 0x00l));
  Canvas.drawCircle(canvas, 320., 240., 30., fill);

  // Creating empty shader
  let _emptyShader = Skia.Shader.makeEmpty();

  let red = Color.makeArgb(0xFFl, 0xFFl, 0x00l, 0x00l);
  let green = Color.makeArgb(0xFFl, 0x00l, 0xFFl, 0x00l);
  let blue = Color.makeArgb(0xFFl, 0x00l, 0x00l, 0xFFl);

  // Creating a 2-stop linear gradient
  let linearGradient2 =
    Skia.Shader.makeLinearGradient2(
      ~startPoint=Skia.Point.make(0.0, 0.0),
      ~stopPoint=Skia.Point.make(100.0, 100.0),
      ~startColor=red,
      ~stopColor=blue,
      ~tileMode=`repeat,
    );
  let fill = Paint.make();
  Paint.setColor(fill, Color.makeArgb(0xFFl, 0xFFl, 0x00l, 0x00l));
  Paint.setShader(fill, linearGradient2);

  Canvas.drawRectLtwh(canvas, 0., 0., 100., 100., fill);

  // Creating a 3-stop linear gradient
  let linearGradient3 =
    Skia.Shader.makeLinearGradient(
      ~startPoint=Skia.Point.make(0.0, 0.0),
      ~stopPoint=Skia.Point.make(100.0, 0.0),
      ~colorStops=
        Skia.Shader.[
          {color: red, position: 0.0},
          {color: blue, position: 0.25},
          {color: green, position: 1.0},
        ],
      ~tileMode=`repeat,
    );
  let fill = Paint.make();
  Paint.setShader(fill, linearGradient3);
  Canvas.drawRectLtwh(canvas, 100., 100., 100., 100., fill);
};

let surface = makeSurface(640l, 480l);
let canvas = Surface.getCanvas(surface);
draw(canvas);
emitPng("skia-c-example.png", surface);
print_endline("Done!");
