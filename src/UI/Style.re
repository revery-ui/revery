open Layout;

open Revery_Core;

type fontFamily = string;

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
  transform: list(Transform.t),
  opacity: float,
};

let make =
    (
      ~backgroundColor: Color.t=Colors.black,
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
      ~transform=[],
      ~opacity=1.0,
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
    opacity,
  };

  ret;
};

let defaultStyle = make();

let scale = (v: int, scaleFactor: int) =>
  v == Encoding.cssUndefined ? Encoding.cssUndefined : scaleFactor * v;

let toLayoutNode = (s: t, scaleFactor: int) => {
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
  };
  ret;
};
