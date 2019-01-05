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
    inset: bool,
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
        ~inset=false,
        (),
      ) => {
    color,
    inset,
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
      ~flexShrink=1,
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

let scale = (v: int, scaleFactor: float) =>
  v == Encoding.cssUndefined ? Encoding.cssUndefined : int_of_float(scaleFactor *. float_of_int(v));

let toLayoutNode = (s: t, scaleFactor: float) => {
  let ret: LayoutTypes.cssStyle = {
    ...LayoutSupport.defaultStyle,
    positionType: s.position,
    top: scale(s.top, scaleFactor),
    left: scale(s.left, scaleFactor),
    bottom: scale(s.bottom, scaleFactor),
    flexBasis: scale(s.flexBasis, scaleFactor),
    flexDirection: s.flexDirection,
    flexGrow: s.flexGrow,
    flexShrink: s.flexShrink,
    alignItems: s.alignItems,
    justifyContent: s.justifyContent,
    right: scale(s.right, scaleFactor),
    width: scale(s.width, scaleFactor),
    height: scale(s.height, scaleFactor),
    marginTop: scale(s.marginTop, scaleFactor),
    marginLeft: scale(s.marginLeft, scaleFactor),
    marginRight: scale(s.marginRight, scaleFactor),
    marginBottom: scale(s.marginBottom, scaleFactor),
    margin: scale(s.margin, scaleFactor),
    marginVertical: scale(s.marginVertical, scaleFactor),
    marginHorizontal: scale(s.marginHorizontal, scaleFactor),
    borderTop: scale(s.borderTop.width, scaleFactor),
    borderLeft: scale(s.borderLeft.width, scaleFactor),
    borderRight: scale(s.borderRight.width, scaleFactor),
    borderBottom: scale(s.borderBottom.width, scaleFactor),
    border: scale(s.border.width, scaleFactor),
    borderHorizontal: scale(s.borderHorizontal.width, scaleFactor),
    borderVertical: scale(s.borderVertical.width, scaleFactor),
  };
  ret;
};
