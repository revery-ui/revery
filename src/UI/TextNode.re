open Revery_Draw;

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
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

    let window = Ui.getActiveWindow();

    let lineHeightPx =
      Text.getLineHeight(~window, ~fontFamily, ~fontSize, ~lineHeight, ());

    /* when style.width & style.height are defined, Layout doesn't call the measure function */
    if (!_isMeasured) {
      _this#measure(style.width, style.height) |> ignore;
    };

    List.iteri(
      (lineNum, line) =>
        Text.drawString(
          ~window,
          ~fontFamily,
          ~fontSize,
          ~gamma,
          ~color,
          ~backgroundColor,
          ~opacity,
          ~transform=_this#getWorldTransform(),
          ~x=0.,
          ~y=lineHeightPx *. float_of_int(lineNum),
          line,
        ),
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

    let formattedText = TextOverflow.removeLineBreaks(text);

    let window = Ui.getActiveWindow();

    let measure = str =>
      Text.measure(~window, ~fontFamily, ~fontSize, str)
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
      Text.getLineHeight(~window, ~fontFamily, ~fontSize, ~lineHeight, ());

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
      Text.getLineHeight(~window, ~fontFamily, ~fontSize, ~lineHeight, ());

    let measureWidth = str =>
      Text.measureCharWidth(~window, ~fontFamily, ~fontSize, str);
    _lines =
      TextWrapping.wrapText(
        ~text,
        ~measureWidth,
        ~maxWidth=float_of_int(width),
        ~mode=textWrap,
      );

    let pickWiderLine = (leftWidth, right) => {
      let rightWidth =
        Text.measure(~window, ~fontFamily, ~fontSize, right).width;
      max(leftWidth, rightWidth);
    };
    let maxWidthLine = List.fold_left(pickWiderLine, 0, _lines);
    {
      width: maxWidthLine,
      height:
        int_of_float(float_of_int(List.length(_lines)) *. lineHeightPx),
    };
  };
  pub! getMeasureFunction = () => {
    let measure =
        (_mode, width, _widthMeasureMode, height, _heightMeasureMode) =>
      _this#measure(width, height);
    Some(measure);
  };
};
