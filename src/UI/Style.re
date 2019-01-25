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
  position: LayoutTypes.positionType,
  flexGrow: int,
  flexBasis: int,
  flexShrink: int,
  flexDirection: LayoutTypes.flexDirection,
  justifyContent: LayoutTypes.justify,
  alignItems: LayoutTypes.align,
  top: int,
  bottom: int,
  left: int,
  right: int,
  fontFamily,
  fontSize: int,
  marginTop: int,
  marginLeft: int,
  marginRight: int,
  marginBottom: int,
  margin: int,
  marginVertical: int,
  marginHorizontal: int,
  overflow: LayoutTypes.overflow,
  borderTop: Border.t,
  borderLeft: Border.t,
  borderRight: Border.t,
  borderBottom: Border.t,
  border: Border.t,
  borderHorizontal: Border.t,
  borderVertical: Border.t,
  transform: list(Transform.t),
  opacity: float,
  boxShadow: BoxShadow.properties,
  cursor: option(MouseCursors.t),
};

let make =
    (
      ~backgroundColor: Color.t=Colors.transparentBlack,
      ~color: Color.t=Colors.white,
      ~width=Encoding.cssUndefined,
      ~height=Encoding.cssUndefined,
      ~flexBasis=Encoding.cssUndefined,
      ~flexDirection=LayoutTypes.Column,
      ~flexGrow=0,
      ~flexShrink=0,
      ~alignItems=LayoutTypes.AlignStretch,
      ~justifyContent=LayoutTypes.JustifyFlexStart,
      ~position=LayoutTypes.Relative,
      ~top=Encoding.cssUndefined,
      ~bottom=Encoding.cssUndefined,
      ~left=Encoding.cssUndefined,
      ~right=Encoding.cssUndefined,
      ~fontFamily="",
      ~fontSize=Encoding.cssUndefined,
      ~marginTop=Encoding.cssUndefined,
      ~marginLeft=Encoding.cssUndefined,
      ~marginRight=Encoding.cssUndefined,
      ~marginBottom=Encoding.cssUndefined,
      ~margin=Encoding.cssUndefined,
      ~marginVertical=Encoding.cssUndefined,
      ~marginHorizontal=Encoding.cssUndefined,
      ~overflow=LayoutTypes.Visible,
      ~borderTop=Border.make(),
      ~borderLeft=Border.make(),
      ~borderRight=Border.make(),
      ~borderBottom=Border.make(),
      ~border=Border.make(),
      ~borderHorizontal=Border.make(),
      ~borderVertical=Border.make(),
      ~transform=[],
      ~opacity=1.0,
      ~boxShadow=BoxShadow.{
                   xOffset: 0.0,
                   yOffset: 0.0,
                   blurRadius: 0.0,
                   spreadRadius: 0.0,
                   color: Colors.black,
                 },
      ~cursor=?,
      _unit: unit,
    ) => {
  let ret: t = {
    backgroundColor,
    color,
    width,
    height,
    flexBasis,
    flexDirection,
    flexGrow,
    flexShrink,
    justifyContent,
    alignItems,
    position,
    top,
    bottom,
    left,
    right,
    fontFamily,
    fontSize,
    transform,
    marginTop,
    marginLeft,
    marginRight,
    marginBottom,
    margin,
    marginVertical,
    marginHorizontal,
    overflow,
    borderTop,
    borderLeft,
    borderRight,
    borderBottom,
    border,
    borderHorizontal,
    borderVertical,
    opacity,
    boxShadow,
    cursor,
  };

  ret;
};

let defaultStyle = make();

let toLayoutNode = (s: t) => {
  let ret: LayoutTypes.cssStyle = {
    ...LayoutSupport.defaultStyle,
    positionType: s.position,
    top: s.top,
    left: s.left,
    bottom: s.bottom,
    flexBasis: s.flexBasis,
    flexDirection: s.flexDirection,
    flexGrow: s.flexGrow,
    flexShrink: s.flexShrink,
    alignItems: s.alignItems,
    justifyContent: s.justifyContent,
    right: s.right,
    width: s.width,
    height: s.height,
    marginTop: s.marginTop,
    marginLeft: s.marginLeft,
    marginRight: s.marginRight,
    marginBottom: s.marginBottom,
    margin: s.margin,
    marginVertical: s.marginVertical,
    marginHorizontal: s.marginHorizontal,
    borderTop: s.borderTop.width,
    borderLeft: s.borderLeft.width,
    borderRight: s.borderRight.width,
    borderBottom: s.borderBottom.width,
    border: s.border.width,
    borderHorizontal: s.borderHorizontal.width,
    borderVertical: s.borderVertical.width,
    overflow: s.overflow,
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
  | `FlexGrow(int)
  | `FlexDirection(LayoutTypes.flexDirection)
  | `JustifyContent(LayoutTypes.justify)
  | `AlignItems(LayoutTypes.align)
  | `Position(LayoutTypes.positionType)
  | `BackgroundColor(Color.t)
  | `Color(Color.t)
  | `Width(int)
  | `Height(int)
  | `Top(int)
  | `Right(int)
  | `Bottom(int)
  | `Left(int)
  | `MarginTop(int)
  | `MarginLeft(int)
  | `MarginRight(int)
  | `MarginBottom(int)
  | `Margin(int)
  | `MarginVertical(int)
  | `MarginHorizontal(int)
  | `Margin2(xy)
  | `Margin4(coords)
  | `Overflow(LayoutTypes.overflow)
  | `BorderTop(Border.t)
  | `BorderLeft(Border.t)
  | `BorderRight(Border.t)
  | `BorderBottom(Border.t)
  | `Border(Border.t)
  | `BorderHorizontal(Border.t)
  | `BorderVertical(Border.t)
  | `Transform(list(Transform.t))
  | `Opacity(float)
  | `BoxShadow(BoxShadow.properties)
  | `Cursor(option(MouseCursors.t))
];

type fontProps = [ | `FontFamily(string) | `FontSize(int)];
/*
   Text and View props take different style properties as such
   these nodes are typed to only allow styles to be specified
   which are relevant to each
 */
type textStyleProps = [ fontProps | coreStyleProps];
type viewStyleProps = [ coreStyleProps];
type imageStyleProps = [ coreStyleProps];

type allProps = [ coreStyleProps | fontProps];

let emptyTextStyle: list(textStyleProps) = [];
let emptyViewStyle: list(viewStyleProps) = [];
let emptyImageStyle: list(imageStyleProps) = [];

let flexDirection = d => {
  let dir =
    switch (d) {
    | `Column => LayoutTypes.Column
    | `ColumnReverse => LayoutTypes.ColumnReverse
    | `RowReverse => LayoutTypes.RowReverse
    | `Row => LayoutTypes.Row
    };
  `FlexDirection(dir);
};

let alignment = a =>
  switch (a) {
  | `Center => LayoutTypes.AlignCenter
  | `Stretch => LayoutTypes.AlignStretch
  | `Auto => LayoutTypes.AlignAuto
  | `FlexStart => LayoutTypes.AlignFlexStart
  | `FlexEnd => LayoutTypes.AlignFlexEnd
  };

let justify = j =>
  switch (j) {
  | `FlexStart => LayoutTypes.JustifyFlexStart
  | `Center => LayoutTypes.JustifyCenter
  | `FlexEnd => LayoutTypes.JustifyFlexEnd
  };

let flexGrow = g => `FlexGrow(g);

let right = f => `Right(f);
let bottom = f => `Bottom(f);
let left = f => `Left(f);
let top = f => `Top(f);

let fontSize = f => `FontSize(f);
let fontFamily = f => `FontFamily(f);

let height = h => `Height(h);
let width = w => `Width(w);

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

let alignItems = a => `AlignItems(alignment(a));
let justifyContent = a => `JustifyContent(justify(a));

let cursor = c => `Cursor(Some(c));

let opacity = o => `Opacity(o);
let transform = t => `Transform(t);
let boxShadow = (~xOffset, ~yOffset, ~spreadRadius, ~blurRadius, ~color) =>
  `BoxShadow(BoxShadow.{xOffset, yOffset, spreadRadius, blurRadius, color});
let overflow = o => `Overflow(o);
let color = o => `Color(o);
let backgroundColor = o => `BackgroundColor(o);

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

/*
   Apply style takes all style props and maps each to the correct style
   and is used to build up the style record, which is eventually
   used to apply styling to elements.
 */
let applyStyle = (style, styleRule: [< coreStyleProps | fontProps]) =>
  switch (styleRule) {
  | `AlignItems(alignItems) => {...style, alignItems}
  | `JustifyContent(justifyContent) => {...style, justifyContent}
  | `FlexGrow(flexGrow) => {...style, flexGrow}
  | `FlexDirection(flexDirection) => {...style, flexDirection}
  | `Position(position) => {...style, position}
  | `Margin(margin) => {...style, margin}
  | `MarginTop(marginTop) => {...style, marginTop}
  | `MarginBottom(marginBottom) => {...style, marginBottom}
  | `MarginRight(marginRight) => {...style, marginRight}
  | `MarginLeft(marginLeft) => {...style, marginLeft}
  | `MarginVertical(marginVertical) => {...style, marginVertical}
  | `MarginHorizontal(marginHorizontal) => {...style, marginHorizontal}
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
  | `Overflow(overflow) => {...style, overflow}
  | `Border(border) => {...style, border}
  | `BorderBottom(borderBottom) => {...style, borderBottom}
  | `BorderTop(borderTop) => {...style, borderTop}
  | `BorderLeft(borderLeft) => {...style, borderLeft}
  | `BorderRight(borderRight) => {...style, borderRight}
  | `BorderVertical(borderVertical) => {...style, borderVertical}
  | `BorderHorizontal(borderHorizontal) => {...style, borderHorizontal}
  | `Opacity(opacity) => {...style, opacity}
  | `BoxShadow(boxShadow) => {...style, boxShadow}
  | `Transform(transform) => {...style, transform}
  | `FontFamily(fontFamily) => {...style, fontFamily}
  | `FontSize(fontSize) => {...style, fontSize}
  | `Cursor(cursor) => {...style, cursor}
  | `Color(color) => {...style, color}
  | `BackgroundColor(backgroundColor) => {...style, backgroundColor}
  | `Width(width) => {...style, width}
  | `Height(height) => {...style, height}
  | `Bottom(bottom) => {...style, bottom}
  | `Left(left) => {...style, left}
  | `Top(top) => {...style, top}
  | `Right(right) => {...style, right}
  };

let create = (~style, ~default=make(), ()) =>
  List.fold_left(applyStyle, default, style);

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
              | (`Overflow(_), `Overflow(_)) => targetStyle
              | (`BorderTop(_), `BorderTop(_)) => targetStyle
              | (`BorderLeft(_), `BorderLeft(_)) => targetStyle
              | (`BorderRight(_), `BorderRight(_)) => targetStyle
              | (`BorderBottom(_), `BorderBottom(_)) => targetStyle
              | (`Border(_), `Border(_)) => targetStyle
              | (`BorderHorizontal(_), `BorderHorizontal(_)) => targetStyle
              | (`BorderVertical(_), `BorderVertical(_)) => targetStyle
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

/* -------------------------------------------------------------------------------*/
