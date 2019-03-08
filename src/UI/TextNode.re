open Revery_Draw;

module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Revery_Core;

open ViewNode;

class textNode (text: string) = {
  as _this;
  val mutable text = text;
  val mutable gamma = 2.2;
  val mutable _isMeasured = false;
  val _lines: ref(list(string)) = ref([]);
  inherit (class viewNode)() as _super;
  pub! draw = (parentContext: NodeDrawContext.t) => {
    /* Draw background first */
    _super#draw(parentContext);

    let style = _super#getStyle();

    let color = Color.multiplyAlpha(parentContext.opacity, style.color);
    let backgroundColor =
      Color.multiplyAlpha(parentContext.opacity, style.backgroundColor);
    let fontFamily = style.fontFamily;
    let fontSize = style.fontSize;
    let lineHeight = style.lineHeight;

    let lineHeightPx =
      Text.getLineHeight(~fontFamily, ~fontSize, ~lineHeight, ());

    /* when style.width & style.height are defined, Layout doesn't call the measure function */
    if (!_isMeasured) {
      _this#measure(style.width, style.height) |> ignore;
    };

    List.iteri(
      (lineNum, line) =>
        Text.drawString(
          ~fontFamily,
          ~fontSize,
          ~gamma,
          ~color,
          ~backgroundColor,
          ~transform=_this#getWorldTransform(),
          ~x=0.,
          ~y=lineHeightPx *. float_of_int(lineNum),
          line,
        ),
      _lines^,
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
  pub textOverflow = maxWidth => {
    let {fontFamily, fontSize, lineHeight, textOverflow, _}: Style.t =
      _super#getStyle();

    let measure = str =>
      Text.measure(~fontFamily, ~fontSize, str)
      |> (value => FontRenderer.(value.width));
    let width = measure(text);
    let isOverflowing = width >= maxWidth;

    let truncated =
      switch (textOverflow, isOverflowing) {
      | (Ellipsis, true) =>
        TextOverflow.handleOverflow(~maxWidth, ~text, ~measure, ())
      | (UserDefined(character), true) =>
        TextOverflow.handleOverflow(
          ~maxWidth,
          ~text,
          ~measure,
          ~character,
          (),
        )
      | (_, false)
      | (Overflow, _) => text
      };

    _lines := [truncated];

    let lineHeightPx =
      Text.getLineHeight(~fontFamily, ~fontSize, ~lineHeight, ());
    let dimensions: Layout.LayoutTypes.dimensions = {
      width,
      height: int_of_float(lineHeightPx),
    };

    dimensions;
  };
  pub setText = t =>
    if (!String.equal(t, text)) {
      text = t;
      _isMeasured = false;
      _this#markLayoutDirty();
    };
  pub measure = (_mode, width, _widthMeasureMode, _height, _heightMeasureMode) => {
    _isMeasured := true;
    /* TODO: Cache font locally in variable */
    let style = _super#getStyle();
    switch (style) {
    | {textOverflow: Ellipsis | UserDefined(_), _} =>
      _this#textOverflow(width)
    | s => _this#handleTextWrapping(width, s)
    };
  };
  pub handleTextWrapping = (width, style) => {
    let {textWrap, fontFamily, fontSize, lineHeight, _}: Style.t = style;
    let lineHeightPx =
      Text.getLineHeight(~fontFamily, ~fontSize, ~lineHeight, ());

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

      _lines := lines;

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

      _lines := [text];

      dimensions;
    | UserDefined(wrapFunc) =>
      let (lines, maxWidthLine) =
        wrapFunc(
          text,
          str => Text.measure(~fontFamily, ~fontSize, str).width,
          width,
        );

      _lines := lines;

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
