open Revery_Draw;

module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Revery_Core;

open Style;
open ViewNode;

class textNode (text: string) = {
  as _this;
  val mutable text = text;
  val mutable gamma = 2.2;
  val mutable _isMeasured = false;
  val mutable _lines: list(string) = [];
  inherit (class viewNode)() as _super;
  pub! draw = (parentContext: NodeDrawContext.t) => {
    /* Draw background first */
    _super#draw(parentContext);

    let style = _super#getStyle();

    let {color, backgroundColor, fontFamily, fontSize, lineHeight, _} = style;
    let opacity = parentContext.opacity *. style.opacity;
    let colorWithAppliedOpacity = Color.multiplyAlpha(opacity, color);

    let ascentPx =
      Text.getAscent(~fontFamily, ~fontSize, ());
    let descentPx =
      Text.getDescent(~fontFamily, ~fontSize, ());
    let lineHeightPx =
      lineHeight *. Text.getLineHeight(~fontFamily, ~fontSize, ());
    let leadingPx = lineHeightPx -. ascentPx -. descentPx;

    /* when style.width & style.height are defined, Layout doesn't call the measure function */
    if (!_isMeasured) {
      _this#measure(style.width, style.height) |> ignore;
    };

    
    let { canvas, _ }: NodeDrawContext.t = parentContext;
    // TODO find a way to only manage the matrix stack in Node
    let world = _this#getWorldTransform();
    let skiaWorld = Revery_Math.Matrix.toSkiaMatrix(world);
    Revery_Draw.Canvas.setMatrix(canvas, skiaWorld);
    
    List.iteri(
      (lineIndex, line) => {
          let baselineY = (leadingPx /. 2. *. 0.) +. ascentPx +. (lineHeightPx *. float_of_int(lineIndex));

          // let dimensions = _this#measurements();
          // // print_endline ("Drawing text: " ++ line ++ " - y: " ++ string_of_float(y));
          // let emHeightPx = ascentPx +. descentPx;       
          // print_endline("fontSize: " ++ string_of_int(fontSize) ++ ", emHeightPx: " ++ string_of_float(emHeightPx));
          // let lineRect = Revery_Math.Rectangle.create(~x=0., ~y=baselineY -. ascentPx, ~width=float_of_int(dimensions.width), ~height=emHeightPx, ());
          // let backgroundColor = Skia.Color.makeArgb(0xAA, 0x33, 0xFF, 0x33);
          // let backgroundPaint = Skia.Paint.make();      
          // Skia.Paint.setColor(backgroundPaint, backgroundColor);
          // Canvas.drawRect(canvas, lineRect, backgroundPaint);

          // let leadingRect = Revery_Math.Rectangle.create(~x=0., ~y=baselineY +. descentPx, ~width=float_of_int(dimensions.width), ~height=leadingPx, ());
          // let leadingColor = Skia.Color.makeArgb(0xAA, 0xFF, 0x33, 0x33);
          // let leadingPaint = Skia.Paint.make();
          // Skia.Paint.setColor(leadingPaint, leadingColor);
          // Canvas.drawRect(canvas, leadingRect, leadingPaint)

          // print_endline ("Drawing text: " ++ line ++ " - y: " ++ string_of_float(y));
          Canvas.drawText(~color=colorWithAppliedOpacity, ~x=0., ~y=baselineY, ~fontFamily, ~fontSize=float_of_int(fontSize), line, canvas);
        }
        ,
      _lines,
    );
  };
  pub setGamma = g => gamma = g;
  pub! setStyle = style => {
    let lastStyle = _this#getStyle();
    _super#setStyle(style);
    let newStyle = _this#getStyle();

    if (lastStyle.lineHeight != newStyle.lineHeight
        || lastStyle.fontSize != newStyle.fontSize
        || !String.equal(lastStyle.fontFamily, newStyle.fontFamily)) {
      _this#markLayoutDirty();
    };
  };
  pub textOverflow = (maxWidth): LayoutTypes.dimensions => {
    let {fontFamily, fontSize, lineHeight, textOverflow, _}: Style.t =
      _super#getStyle();
    let window = Ui.getActiveWindow();

    let formattedText = TextOverflow.removeLineBreaks(text);

    let window = Ui.getActiveWindow();

    let measure = str =>
      Text.measure(~fontFamily, ~fontSize, str)
      |> (value => value.width);

    let width = measure(formattedText);
    let isOverflowing = width >= maxWidth;

    let handleOverflow =
      TextOverflow.handleOverflow(~maxWidth, ~text=formattedText, ~measure);

    let truncated =
      switch (textOverflow, isOverflowing) {
      | (Ellipsis, true) => handleOverflow()
      | (UserDefined(character), true) => handleOverflow(~character, ())
      | (Clip, true) => handleOverflow(~character="", ())
      | (_, false)
      | (Overflow, _) => text
      };

    _lines = [truncated];

    let lineHeightPx =
      lineHeight *. Text.getLineHeight(~fontFamily, ~fontSize, ());

    {width, height: int_of_float(lineHeightPx)};
  };
  pub setText = t =>
    if (!String.equal(t, text)) {
      text = t;
      _isMeasured = false;
      _this#markLayoutDirty();
    };
  pub measure = (width, _height) => {
    _isMeasured = true;
    /**
         If the width value is set to cssUndefined i.e. the user did not
         set a width then do not attempt to use textOverflow
       */
    (
      switch (_super#getStyle()) {
      | {width: textWidth, _} as style
          when textWidth == Layout.Encoding.cssUndefined =>
        _this#handleTextWrapping(width, style)
      | {textOverflow: Ellipsis | UserDefined(_), _} =>
        _this#textOverflow(width)
      | style => _this#handleTextWrapping(width, style)
      }
    );
  };
  pub handleTextWrapping = (width, style) => {
    let {textWrap, fontFamily, fontSize, lineHeight, _}: Style.t = style;
    let window = Ui.getActiveWindow();
    let lineHeightPx =
      lineHeight *. Text.getLineHeight(~fontFamily, ~fontSize, ());

    switch (textWrap) {
    | WhitespaceWrap =>
      let (lines, maxWidthLine) =
        TextWrapping.wrapText(
          ~text,
          ~measureWidth=
            str => Text.measure(~fontFamily, ~fontSize, str).width,
          ~maxWidth=width,
          ~wrapHere=TextWrapping.isWhitespaceWrapPoint,
        );

      _lines = lines;

      let dimensions: Layout.LayoutTypes.dimensions = {
        width: int_of_float(float_of_int(maxWidthLine)),
        height:
          int_of_float(float_of_int(List.length(lines)) *. lineHeightPx),
      };

      dimensions;
    | NoWrap =>
      let d = Text.measure(~fontFamily, ~fontSize, text);
      let dimensions: Layout.LayoutTypes.dimensions = {
        width: d.width,
        height: d.height,
      };

      _lines = [text];

      dimensions;
    | UserDefined(wrapFunc) =>
      let (lines, maxWidthLine) =
        wrapFunc(
          text,
          str => Text.measure(~fontFamily, ~fontSize, str).width,
          width,
        );

      _lines = lines;

      let dimensions: Layout.LayoutTypes.dimensions = {
        width: maxWidthLine,
        height:
          int_of_float(float_of_int(List.length(lines)) *. lineHeightPx),
      };

      dimensions;
    };
  };
  pub! getMeasureFunction = () => {
    let measure =
        (_mode, width, _widthMeasureMode, height, _heightMeasureMode) =>
      _this#measure(width, height);
    Some(measure);
  };
};
