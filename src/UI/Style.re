open Layout;

open Revery_Core;

type fontFamily = string;

type t = {
  backgroundColor: Color.t,
  color: Color.t,
  width: int,
  height: int,
  position: LayoutTypes.positionType,
  flexDirection: LayoutTypes.flexDirection,
  top: int,
  bottom: int,
  left: int,
  right: int,
  fontFamily,
  fontSize: int,
  transform: list(Transform.t),
};

let make =
    (
      ~backgroundColor: Color.t=Colors.black,
      ~color: Color.t=Colors.white,
      ~width=Encoding.cssUndefined,
      ~height=Encoding.cssUndefined,
      ~flexDirection=LayoutTypes.Column,
      ~position=LayoutTypes.Relative,
      ~top=Encoding.cssUndefined,
      ~bottom=Encoding.cssUndefined,
      ~left=Encoding.cssUndefined,
      ~right=Encoding.cssUndefined,
      ~fontFamily="",
      ~fontSize=Encoding.cssUndefined,
      ~transform=[],
      _unit: unit,
    ) => {
  let ret: t = {
    backgroundColor,
    color,
    width,
    height,
    flexDirection,
    position,
    top,
    bottom,
    left,
    right,
    fontFamily,
    fontSize,
    transform,
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
    flexDirection: s.flexDirection,
    right: s.right,
    width: s.width,
    height: s.height,
  };
  ret;
};
