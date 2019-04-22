open Layout;

open Revery_Core;

type fontFamily = string;

module Border = {
  type t = {
    color: Color.t,
    width: int,
  };

  let make = (~color=Colors.black, ~width=Encoding.cssUndefined, ()) => {
    color,
    width,
  };
};

module BoxShadow = {
  type properties = {
    xOffset: float,
    yOffset: float,
    blurRadius: float,
    spreadRadius: float,
    color: Color.t,
  };
};

type t = {
  backgroundColor: Color.t,
  color: Color.t,
  width: int,
  height: int,
  hover: option(t),
  position: LayoutTypes.positionType,
  flexGrow: int,
  flexBasis: int,
  flexShrink: int,
  flexDirection: LayoutTypes.flexDirection,
  flexWrap: LayoutTypes.wrapType,
  justifyContent: LayoutTypes.justify,
  alignItems: LayoutTypes.align,
  alignSelf: LayoutTypes.align,
  top: int,
  bottom: int,
  left: int,
  right: int,
  fontFamily,
  fontSize: int,
  lineHeight: float,
  textWrap: TextWrapping.wrapType,
  marginTop: int,
  marginLeft: int,
  marginRight: int,
  marginBottom: int,
  margin: int,
  marginVertical: int,
  marginHorizontal: int,
  minWidth: int,
  maxWidth: int,
  minHeight: int,
  maxHeight: int,
  paddingTop: int,
  paddingLeft: int,
  paddingRight: int,
  paddingBottom: int,
  padding: int,
  paddingVertical: int,
  paddingHorizontal: int,
  textOverflow: TextOverflow.t,
  overflow: LayoutTypes.overflow,
  borderTop: Border.t,
  borderLeft: Border.t,
  borderRight: Border.t,
  borderBottom: Border.t,
  border: Border.t,
  borderHorizontal: Border.t,
  borderVertical: Border.t,
  borderRadius: float,
  transform: list(Transform.t),
  opacity: float,
  boxShadow: BoxShadow.properties,
  cursor: option(MouseCursors.t),
};

let make =
    (
      ~alignItems=LayoutTypes.AlignStretch,
      ~alignSelf=LayoutTypes.AlignAuto,
      ~backgroundColor: Color.t=Colors.transparentBlack,
      ~border=Border.make(),
      ~borderBottom=Border.make(),
      ~borderHorizontal=Border.make(),
      ~borderLeft=Border.make(),
      ~borderRadius=0.0,
      ~borderRight=Border.make(),
      ~borderTop=Border.make(),
      ~borderVertical=Border.make(),
      ~bottom=Encoding.cssUndefined,
      ~boxShadow=BoxShadow.{
                   xOffset: 0.0,
                   yOffset: 0.0,
                   blurRadius: 0.0,
                   spreadRadius: 0.0,
                   color: Colors.black,
                 },
      ~color: Color.t=Colors.white,
      ~cursor=?,
      ~flexBasis=Encoding.cssUndefined,
      ~flexDirection=LayoutTypes.Column,
      ~flexGrow=0,
      ~flexShrink=0,
      ~flexWrap=LayoutTypes.CssNoWrap,
      ~fontFamily="",
      ~fontSize=Encoding.cssUndefined,
      ~height=Encoding.cssUndefined,
      ~hover=None,
      ~justifyContent=LayoutTypes.JustifyFlexStart,
      ~left=Encoding.cssUndefined,
      ~lineHeight=1.2,
      ~margin=Encoding.cssUndefined,
      ~marginBottom=Encoding.cssUndefined,
      ~marginHorizontal=Encoding.cssUndefined,
      ~marginLeft=Encoding.cssUndefined,
      ~marginRight=Encoding.cssUndefined,
      ~marginTop=Encoding.cssUndefined,
      ~marginVertical=Encoding.cssUndefined,
      ~maxHeight=Encoding.cssUndefined,
      ~maxWidth=Encoding.cssUndefined,
      ~minHeight=Encoding.cssUndefined,
      ~minWidth=Encoding.cssUndefined,
      ~opacity=1.0,
      ~overflow=LayoutTypes.Visible,
      ~padding=Encoding.cssUndefined,
      ~paddingBottom=Encoding.cssUndefined,
      ~paddingHorizontal=Encoding.cssUndefined,
      ~paddingLeft=Encoding.cssUndefined,
      ~paddingRight=Encoding.cssUndefined,
      ~paddingTop=Encoding.cssUndefined,
      ~paddingVertical=Encoding.cssUndefined,
      ~position=LayoutTypes.Relative,
      ~right=Encoding.cssUndefined,
      ~textOverflow=TextOverflow.Overflow,
      ~textWrap=TextWrapping.WhitespaceWrap,
      ~top=Encoding.cssUndefined,
      ~transform=[],
      ~width=Encoding.cssUndefined,
      _unit: unit,
    ) => {
  let ret: t = {
    alignItems,
    alignSelf,
    backgroundColor,
    border,
    borderBottom,
    borderHorizontal,
    borderLeft,
    borderRadius,
    borderRight,
    borderTop,
    borderVertical,
    bottom,
    boxShadow,
    color,
    cursor,
    flexBasis,
    flexDirection,
    flexGrow,
    flexShrink,
    flexWrap,
    fontFamily,
    fontSize,
    height,
    hover,
    justifyContent,
    left,
    lineHeight,
    margin,
    marginBottom,
    marginHorizontal,
    marginLeft,
    marginRight,
    marginTop,
    marginVertical,
    maxHeight,
    maxWidth,
    minHeight,
    minWidth,
    opacity,
    overflow,
    padding,
    paddingBottom,
    paddingHorizontal,
    paddingLeft,
    paddingRight,
    paddingTop,
    paddingVertical,
    position,
    right,
    textOverflow,
    textWrap,
    top,
    transform,
    width,
  };

  ret;
};

let defaultStyle = make();

let toLayoutNode = (s: t) => {
  let ret: LayoutTypes.cssStyle = {
    ...LayoutSupport.defaultStyle,
    alignItems: s.alignItems,
    alignSelf: s.alignSelf,
    border: s.border.width,
    borderBottom: s.borderBottom.width,
    borderHorizontal: s.borderHorizontal.width,
    borderLeft: s.borderLeft.width,
    borderRight: s.borderRight.width,
    borderTop: s.borderTop.width,
    borderVertical: s.borderVertical.width,
    bottom: s.bottom,
    flexBasis: s.flexBasis,
    flexDirection: s.flexDirection,
    flexGrow: s.flexGrow,
    flexShrink: s.flexShrink,
    flexWrap: s.flexWrap,
    height: s.height,
    justifyContent: s.justifyContent,
    left: s.left,
    margin: s.margin,
    marginBottom: s.marginBottom,
    marginHorizontal: s.marginHorizontal,
    marginLeft: s.marginLeft,
    marginRight: s.marginRight,
    marginTop: s.marginTop,
    marginVertical: s.marginVertical,
    maxHeight: s.maxHeight,
    maxWidth: s.maxWidth,
    minHeight: s.minHeight,
    minWidth: s.minWidth,
    overflow: s.overflow,
    padding: s.padding,
    paddingBottom: s.paddingBottom,
    paddingHorizontal: s.paddingHorizontal,
    paddingLeft: s.paddingLeft,
    paddingRight: s.paddingRight,
    paddingTop: s.paddingTop,
    paddingVertical: s.paddingVertical,
    positionType: s.position,
    right: s.right,
    top: s.top,
    width: s.width,
  };
  ret;
};

/* -------------------------------------------------------------------------------
        Styles: As a list of Polymorphic variants
   -------------------------------------------------------------------------------*/
type coords = {
  top: int,
  right: int,
  bottom: int,
  left: int,
};

type xy = {
  horizontal: int,
  vertical: int,
};

type coreStyleProps = [
  | `AlignItems(LayoutTypes.align)
  | `AlignSelf(LayoutTypes.align)
  | `BackgroundColor(Color.t)
  | `Border(Border.t)
  | `BorderBottom(Border.t)
  | `BorderHorizontal(Border.t)
  | `BorderLeft(Border.t)
  | `BorderRadius(float)
  | `BorderRight(Border.t)
  | `BorderTop(Border.t)
  | `BorderVertical(Border.t)
  | `Bottom(int)
  | `BoxShadow(BoxShadow.properties)
  | `Color(Color.t)
  | `Cursor(option(MouseCursors.t))
  | `FlexDirection(LayoutTypes.flexDirection)
  | `FlexGrow(int)
  | `FlexWrap(LayoutTypes.wrapType)
  | `Height(int)
  | `JustifyContent(LayoutTypes.justify)
  | `Left(int)
  | `Margin(int)
  | `Margin2(xy)
  | `Margin4(coords)
  | `MarginBottom(int)
  | `MarginHorizontal(int)
  | `MarginLeft(int)
  | `MarginRight(int)
  | `MarginTop(int)
  | `MarginVertical(int)
  | `MaxHeight(int)
  | `MaxWidth(int)
  | `MinHeight(int)
  | `MinWidth(int)
  | `Opacity(float)
  | `Overflow(LayoutTypes.overflow)
  | `Padding(int)
  | `Padding2(xy)
  | `Padding4(coords)
  | `PaddingBottom(int)
  | `PaddingHorizontal(int)
  | `PaddingLeft(int)
  | `PaddingRight(int)
  | `PaddingTop(int)
  | `PaddingVertical(int)
  | `Position(LayoutTypes.positionType)
  | `Right(int)
  | `Top(int)
  | `Transform(list(Transform.t))
  | `Width(int)
];

type fontProps = [ | `FontFamily(string) | `FontSize(int)];

type textProps = [
  | `LineHeight(float)
  | `TextOverflow(TextOverflow.t)
  | `TextWrap(TextWrapping.wrapType)
];

type pseudoProps('t) = [ | `Hover('t)];

/*
   Text and View props take different style properties as such
   these nodes are typed to only allow styles to be specified
   which are relevant to each
 */

type textStyleProps = [
  textProps
  | fontProps
  | coreStyleProps
  | pseudoProps(list(textStyleProps))
];
type viewStyleProps = [ coreStyleProps];
type imageStyleProps = [ coreStyleProps];

type allProps = [ coreStyleProps | fontProps | textProps];

let emptyTextStyle: list(textStyleProps) = [];
let emptyViewStyle: list(viewStyleProps) = [];
let emptyImageStyle: list(imageStyleProps) = [];

let flexDirection = d => {
  let dir =
    switch (d) {
    | `Column => LayoutTypes.Column
    | `ColumnReverse => LayoutTypes.ColumnReverse
    | `Row => LayoutTypes.Row
    | `RowReverse => LayoutTypes.RowReverse
    };
  `FlexDirection(dir);
};

let flexWrap = w => {
  let wrap =
    switch (w) {
    | `Wrap => LayoutTypes.CssWrap
    | `NoWrap => LayoutTypes.CssNoWrap
    };
  `FlexWrap(wrap);
};

let textOverflow = overflow =>
  (
    switch (overflow) {
    | `Ellipsis => TextOverflow.Ellipsis
    | `Overflow => TextOverflow.Overflow
    | `UserDefined(v) => TextOverflow.UserDefined(v)
    }
  )
  |> (s => `TextOverflow(s));

let alignment = a =>
  switch (a) {
  | `Auto => LayoutTypes.AlignAuto
  | `Center => LayoutTypes.AlignCenter
  | `FlexEnd => LayoutTypes.AlignFlexEnd
  | `FlexStart => LayoutTypes.AlignFlexStart
  | `Stretch => LayoutTypes.AlignStretch
  };

let justify = j =>
  switch (j) {
  | `Center => LayoutTypes.JustifyCenter
  | `FlexEnd => LayoutTypes.JustifyFlexEnd
  | `FlexStart => LayoutTypes.JustifyFlexStart
  | `SpaceAround => LayoutTypes.JustifySpaceAround
  | `SpaceBetween => LayoutTypes.JustifySpaceBetween
  };

let flexGrow = g => `FlexGrow(g);

let right = f => `Right(f);
let bottom = f => `Bottom(f);
let left = f => `Left(f);
let top = f => `Top(f);

let fontSize = f => `FontSize(f);
let fontFamily = f => `FontFamily(f);
let lineHeight = h => `LineHeight(h);
let textWrap = w => `TextWrap(w);

let height = h => `Height(h);
let width = w => `Width(w);

let minWidth = w => `MinWidth(w);
let maxWidth = w => `MaxWidth(w);
let minHeight = h => `MinHeight(h);
let maxHeight = h => `MaxHeight(h);

let position = p => {
  let value =
    switch (p) {
    | `Absolute => LayoutTypes.Absolute
    | `Relative => LayoutTypes.Relative
    };
  `Position(value);
};

let margin = m => `Margin(m);
let marginLeft = m => `MarginLeft(m);
let marginRight = m => `MarginRight(m);
let marginTop = m => `MarginTop(m);
let marginBottom = m => `MarginBottom(m);
let marginVertical = m => `MarginVertical(m);
let marginHorizontal = m => `MarginHorizontal(m);
let margin2 = (~horizontal, ~vertical) => `Margin2({horizontal, vertical});
let margin4 = (~top, ~right, ~bottom, ~left) =>
  `Margin4({top, right, bottom, left});

let padding = m => `Padding(m);
let paddingLeft = m => `PaddingLeft(m);
let paddingRight = m => `PaddingRight(m);
let paddingTop = m => `PaddingTop(m);
let paddingBottom = m => `PaddingBottom(m);
let paddingVertical = m => `PaddingVertical(m);
let paddingHorizontal = m => `PaddingHorizontal(m);
let padding2 = (~horizontal, ~vertical) => `Padding2({horizontal, vertical});
let padding4 = (~top, ~right, ~bottom, ~left) =>
  `Padding4({top, right, bottom, left});

let border = (~color, ~width) =>
  Border.make(~color, ~width, ()) |> (b => `Border(b));
let borderLeft = (~color, ~width) =>
  Border.make(~color, ~width, ()) |> (b => `BorderLeft(b));
let borderRight = (~color, ~width) =>
  Border.make(~color, ~width, ()) |> (b => `BorderRight(b));
let borderTop = (~color, ~width) =>
  Border.make(~color, ~width, ()) |> (b => `BorderTop(b));
let borderBottom = (~color, ~width) =>
  Border.make(~color, ~width, ()) |> (b => `BorderBottom(b));
let borderHorizontal = (~color, ~width) =>
  Border.make(~color, ~width, ()) |> (b => `BorderHorizontal(b));
let borderVertical = (~color, ~width) =>
  Border.make(~color, ~width, ()) |> (b => `BorderVertical(b));

let borderRadius = r => `BorderRadius(r);

let alignItems = a => `AlignItems(alignment(a));
let justifyContent = a => `JustifyContent(justify(a));
let alignSelf = a => `AlignSelf(alignment(a));

let cursor = c => `Cursor(Some(c));

let opacity = o => `Opacity(o);
let transform = t => `Transform(t);
let boxShadow = (~xOffset, ~yOffset, ~spreadRadius, ~blurRadius, ~color) =>
  `BoxShadow(BoxShadow.{xOffset, yOffset, spreadRadius, blurRadius, color});

let overflow = o =>
  switch (o) {
  | `Visible => `Overflow(LayoutTypes.Visible)
  | `Hidden => `Overflow(LayoutTypes.Hidden)
  | `Scroll => `Overflow(LayoutTypes.Scroll)
  };

let color = o => `Color(o);
let backgroundColor = o => `BackgroundColor(o);

let hover = h => `Hover(h);

/*
   Helper function to narrow down a list of all possible style props to
   one specific to a type of component
 */
let rec extractViewStyles = (styles: list(allProps)): list(viewStyleProps) =>
  switch (styles) {
  | [] => []
  | [#viewStyleProps as v, ...list] => [v, ...extractViewStyles(list)]
  | [_, ...list] => extractViewStyles(list)
  };

let extractPseudoStyles = (~styles, ~pseudoState) =>
  switch (pseudoState) {
  | `Hover =>
    try (
      styles
      |> List.find(
           fun
           | `Hover(_) => true
           | _ => false,
         )
      |> (
        fun
        | `Hover(h) => h
        | _ => []
      )
    ) {
    | Not_found => []
    }
  | `Idle => []
  };

/*
   Apply style takes all style props and maps each to the correct style
   and is used to build up the style record, which is eventually
   used to apply styling to elements.
 */
let rec applyStyle = (style, styleRule) =>
  switch (styleRule) {
  | `AlignItems(alignItems) => {...style, alignItems}
  | `AlignSelf(alignSelf) => {...style, alignSelf}
  | `FlexDirection(flexDirection) => {...style, flexDirection}
  | `FlexGrow(flexGrow) => {...style, flexGrow}
  | `FlexWrap(flexWrap) => {...style, flexWrap}
  | `JustifyContent(justifyContent) => {...style, justifyContent}
  | `Margin(margin) => {...style, margin}
  | `MarginBottom(marginBottom) => {...style, marginBottom}
  | `MarginHorizontal(marginHorizontal) => {...style, marginHorizontal}
  | `MarginLeft(marginLeft) => {...style, marginLeft}
  | `MarginRight(marginRight) => {...style, marginRight}
  | `MarginTop(marginTop) => {...style, marginTop}
  | `MarginVertical(marginVertical) => {...style, marginVertical}
  | `Position(position) => {...style, position}
  | `Margin2({horizontal, vertical}) => {
      ...style,
      marginHorizontal: horizontal,
      marginVertical: vertical,
    }
  | `Margin4({top, right, bottom, left}) => {
      ...style,
      marginTop: top,
      marginLeft: left,
      marginRight: right,
      marginBottom: bottom,
    }
  | `MaxHeight(maxHeight) => {...style, maxHeight}
  | `MaxWidth(maxWidth) => {...style, maxWidth}
  | `MinHeight(minHeight) => {...style, minHeight}
  | `MinWidth(minWidth) => {...style, minWidth}
  | `Padding(padding) => {...style, padding}
  | `PaddingBottom(paddingBottom) => {...style, paddingBottom}
  | `PaddingHorizontal(paddingHorizontal) => {...style, paddingHorizontal}
  | `PaddingLeft(paddingLeft) => {...style, paddingLeft}
  | `PaddingRight(paddingRight) => {...style, paddingRight}
  | `PaddingTop(paddingTop) => {...style, paddingTop}
  | `PaddingVertical(paddingVertical) => {...style, paddingVertical}
  | `Padding2({horizontal, vertical}) => {
      ...style,
      paddingHorizontal: horizontal,
      paddingVertical: vertical,
    }
  | `Padding4({top, right, bottom, left}) => {
      ...style,
      paddingTop: top,
      paddingLeft: left,
      paddingRight: right,
      paddingBottom: bottom,
    }
  | `BackgroundColor(backgroundColor) => {...style, backgroundColor}
  | `Border(border) => {...style, border}
  | `BorderBottom(borderBottom) => {...style, borderBottom}
  | `BorderHorizontal(borderHorizontal) => {...style, borderHorizontal}
  | `BorderLeft(borderLeft) => {...style, borderLeft}
  | `BorderRadius(borderRadius) => {...style, borderRadius}
  | `BorderRight(borderRight) => {...style, borderRight}
  | `BorderTop(borderTop) => {...style, borderTop}
  | `BorderVertical(borderVertical) => {...style, borderVertical}
  | `Bottom(bottom) => {...style, bottom}
  | `BoxShadow(boxShadow) => {...style, boxShadow}
  | `Color(color) => {...style, color}
  | `Cursor(cursor) => {...style, cursor}
  | `FontFamily(fontFamily) => {...style, fontFamily}
  | `FontSize(fontSize) => {...style, fontSize}
  | `Height(height) => {...style, height}
  | `Hover(hoverStyles) => {
      ...style,
      hover: Some(List.fold_left(applyStyle, defaultStyle, hoverStyles)),
    }
  | `Left(left) => {...style, left}
  | `LineHeight(lineHeight) => {...style, lineHeight}
  | `Opacity(opacity) => {...style, opacity}
  | `Overflow(overflow) => {...style, overflow}
  | `Right(right) => {...style, right}
  | `TextOverflow(textOverflow) => {...style, textOverflow}
  | `TextWrap(textWrap) => {...style, textWrap}
  | `Top(top) => {...style, top}
  | `Transform(transform) => {...style, transform}
  | `Width(width) => {...style, width}
  };

let create = (~style, ~default=make(), ()) => {
  List.fold_left(applyStyle, default, style);
};

/*
   This function merges two lists of type styleProps
   the target values override any similar source values

   TODO: is there is a faster/more performant way to do this?
 */
let merge = (~source, ~target) =>
  List.fold_left(
    (merged, targetStyle) => {
      let newStyles =
        List.fold_left(
          (accum, sourceStyle) =>
            (
              switch (targetStyle, sourceStyle) {
              | (`Cursor(_), `Cursor(_)) => targetStyle
              | (`Position(_), `Position(_)) => targetStyle
              | (`BackgroundColor(_), `BackgroundColor(_)) => targetStyle
              | (`Color(_), `Color(_)) => targetStyle
              | (`Width(_), `Width(_)) => targetStyle
              | (`Height(_), `Height(_)) => targetStyle
              | (`Bottom(_), `Bottom(_)) => targetStyle
              | (`Left(_), `Left(_)) => targetStyle
              | (`Right(_), `Right(_)) => targetStyle
              | (`MarginTop(_), `MarginTop(_)) => targetStyle
              | (`MarginLeft(_), `MarginLeft(_)) => targetStyle
              | (`MarginRight(_), `MarginRight(_)) => targetStyle
              | (`MarginBottom(_), `MarginBottom(_)) => targetStyle
              | (`Margin(_), `Margin(_)) => targetStyle
              | (`MarginVertical(_), `MarginVertical(_)) => targetStyle
              | (`MarginHorizontal(_), `MarginHorizontal(_)) => targetStyle
              | (`Margin2(_), `Margin2(_)) => targetStyle
              | (`Margin4(_), `Margin4(_)) => targetStyle
              | (`PaddingTop(_), `PaddingTop(_)) => targetStyle
              | (`PaddingLeft(_), `PaddingLeft(_)) => targetStyle
              | (`PaddingRight(_), `PaddingRight(_)) => targetStyle
              | (`PaddingBottom(_), `PaddingBottom(_)) => targetStyle
              | (`Padding(_), `Padding(_)) => targetStyle
              | (`PaddingVertical(_), `PaddingVertical(_)) => targetStyle
              | (`PaddingHorizontal(_), `PaddingHorizontal(_)) => targetStyle
              | (`Padding2(_), `Padding2(_)) => targetStyle
              | (`Padding4(_), `Padding4(_)) => targetStyle
              | (`Overflow(_), `Overflow(_)) => targetStyle
              | (`BorderTop(_), `BorderTop(_)) => targetStyle
              | (`BorderLeft(_), `BorderLeft(_)) => targetStyle
              | (`BorderRight(_), `BorderRight(_)) => targetStyle
              | (`BorderBottom(_), `BorderBottom(_)) => targetStyle
              | (`Border(_), `Border(_)) => targetStyle
              | (`BorderHorizontal(_), `BorderHorizontal(_)) => targetStyle
              | (`BorderVertical(_), `BorderVertical(_)) => targetStyle
              | (`BorderRadius(_), `BorderRadius(_)) => targetStyle
              | (`Transform(_), `Transform(_)) => targetStyle
              | (`Opacity(_), `Opacity(_)) => targetStyle
              | (`BoxShadow(_), `BoxShadow(_)) => targetStyle
              | (newRule, _) => newRule
              }
            )
            |> (style => [style, ...accum]),
          [],
          source,
        );
      List.append(merged, newStyles);
    },
    source,
    target,
  );
