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
  let make =
      (
        ~xOffset=0.0,
        ~yOffset=0.0,
        ~blurRadius=0.0,
        ~spreadRadius=0.0,
        ~color=Colors.black,
        (),
      ) => {
    color,
    xOffset,
    yOffset,
    blurRadius,
    spreadRadius,
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

let extend =
    (
      style: t,
      ~backgroundColor=style.backgroundColor,
      ~color=style.color,
      ~width=style.width,
      ~height=style.height,
      ~flexBasis=style.flexBasis,
      ~flexDirection=style.flexDirection,
      ~flexGrow=style.flexGrow,
      ~flexShrink=style.flexShrink,
      ~alignItems=style.alignItems,
      ~justifyContent=style.justifyContent,
      ~position=style.position,
      ~top=style.top,
      ~bottom=style.bottom,
      ~left=style.left,
      ~right=style.right,
      ~fontFamily=style.fontFamily,
      ~fontSize=style.fontSize,
      ~marginTop=style.marginTop,
      ~marginLeft=style.marginLeft,
      ~marginRight=style.marginRight,
      ~marginBottom=style.marginBottom,
      ~margin=style.margin,
      ~marginVertical=style.marginVertical,
      ~marginHorizontal=style.marginHorizontal,
      ~borderTop=style.borderTop,
      ~borderLeft=style.borderLeft,
      ~borderRight=style.borderRight,
      ~borderBottom=style.borderBottom,
      ~border=style.border,
      ~borderHorizontal=style.borderHorizontal,
      ~borderVertical=style.borderVertical,
      ~transform=style.transform,
      ~opacity=style.opacity,
      ~overflow=style.overflow,
      ~boxShadow=style.boxShadow,
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
    borderTop,
    borderLeft,
    borderRight,
    borderBottom,
    border,
    borderHorizontal,
    borderVertical,
    opacity,
    overflow,
    boxShadow,
    cursor,
  };
  ret;
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
      ~boxShadow=BoxShadow.make(
                   ~xOffset=0.0,
                   ~yOffset=0.0,
                   ~blurRadius=0.0,
                   ~spreadRadius=0.0,
                   ~color=Colors.black,
                   (),
                 ),
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

type styleProps = [
  | `Position(LayoutTypes.positionType)
  | `BackgroundColor(Color.t)
  | `Color(Color.t)
  | `Width(int)
  | `Height(int)
  | `Bottom(int)
  | `Left(int)
  | `Right(int)
  | `FontFamily(string)
  | `FontSize(int)
  | `MarginTop(int)
  | `MarginLeft(int)
  | `MarginRight(int)
  | `MarginBottom(int)
  | `Margin(int)
  | `MarginVertical(int)
  | `MarginHorizontal(int)
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
  | `Boxshadow(BoxShadow.properties)
  | `Cursor(option(MouseCursors.t))
];

let flexDirection = d =>
  switch (d) {
  | `Column => LayoutTypes.Column
  | `ColumnReverse => LayoutTypes.ColumnReverse
  | `RowReverse => LayoutTypes.RowReverse
  | `Row => LayoutTypes.Row
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

let border = (b: Border.t) =>
  Border.make(~color=b.color, ~width=b.width, ()) |> (b => `Border(b));
let borderLeft = (b: Border.t) =>
  Border.make(~color=b.color, ~width=b.width, ()) |> (b => `BorderLeft(b));
let borderRight = (b: Border.t) =>
  Border.make(~color=b.color, ~width=b.width, ()) |> (b => `BorderRight(b));
let borderTop = (b: Border.t) =>
  Border.make(~color=b.color, ~width=b.width, ()) |> (b => `BorderTop(b));
let borderBottom = (b: Border.t) =>
  Border.make(~color=b.color, ~width=b.width, ()) |> (b => `BorderBottom(b));
let borderHorizontal = (b: Border.t) =>
  Border.make(~color=b.color, ~width=b.width, ())
  |> (b => `BorderHorizontal(b));
let borderVertical = (b: Border.t) =>
  Border.make(~color=b.color, ~width=b.width, ())
  |> (b => `BorderVertical(b));

let alignItems = a => `AlignItems(alignment(a));
let justifyContent = a => `JustifyContent(justify(a));

let cursor = c => `Cursor(Some(c));

let opacity = o => `Opacity(o);
let transform = t => `Transform(t);
let boxShadow = b => `BoxShadow(b);
let overflow = o => `Overflow(o);
let color = o => `Color(o);
let backgroundColor = o => `BackgroundColor(o);

let applyStyle = (style: t, styleRule: [> styleProps]) =>
  switch (styleRule) {
  | `AlignItems(alignItems) => {...style, alignItems}
  | `JustifyContent(justifyContent) => {...style, justifyContent}
  | `FlexDirection(flexDirection) => {...style, flexDirection}
  | `Position(position) => {...style, position}
  | `Margin(margin) => {...style, margin}
  | `MarginTop(marginTop) => {...style, marginTop}
  | `MarginBottom(marginBottom) => {...style, marginBottom}
  | `MarginRight(marginRight) => {...style, marginRight}
  | `MarginLeft(marginLeft) => {...style, marginLeft}
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
  | `MarginVertical(marginVertical) => {...style, marginVertical}
  | `MarginHorizontal(marginHorizontal) => {...style, marginHorizontal}
  | `Color(color) => {...style, color}
  | `BackgroundColor(backgroundColor) => {...style, backgroundColor}
  | `Width(width) => {...style, width}
  | `Height(height) => {...style, height}
  | `Bottom(bottom) => {...style, bottom}
  | `Left(left) => {...style, left}
  | `Top(top) => {...style, top}
  | `Right(right) => {...style, right}
  | _ => style
  };

let create = (~style=[], ~default=make(), ()) =>
  List.fold_left(applyStyle, default, style);

/* let merge = (~source, ~target) => {*/
/*   List.map()*/
/* }*/
/* /* -------------------------------------------------------------------------------*/*/
