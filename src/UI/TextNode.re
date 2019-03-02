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
  val _lines: ref(list(string)) = ref([]);
  inherit (class viewNode)() as _super;
  pub! draw = (parentContext: NodeDrawContext.t) => {
    /* Draw background first */
    _super#draw(parentContext);

    let style = _super#getStyle();

    let color = Color.multiplyAlpha(parentContext.opacity, style.color);
    let fontFamily = style.fontFamily;
    let fontSize = style.fontSize;
    let lineHeight = style.lineHeight;

    let lineHeightPx =
      Text.getLineHeight(~fontFamily, ~fontSize, ~lineHeight, ());

    List.iteri(
      (lineNum, line) =>
        Text.drawString(
          ~fontFamily,
          ~fontSize,
          ~color,
          ~transform=_this#getWorldTransform(),
          ~x=0.,
          ~y=lineHeightPx *. float_of_int(lineNum),
          line,
        ),
      _lines^,
    );
  };
  pub setText = t => text = t;
  pub! getMeasureFunction = (_pixelRatio, _scaleFactor) => {
    let measure =
        (_mode, width, _widthMeasureMode, _height, _heightMeasureMode) => {
      /* TODO: Cache font locally in variable */
      let style = _super#getStyle();
      let textWrap = style.textWrap;

      let fontFamily = style.fontFamily;
      let fontSize = style.fontSize;
      let lineHeight = style.lineHeight;

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
    Some(measure);
  };
};
