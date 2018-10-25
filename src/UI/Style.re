open Layout;

open Revery_Core;

type t = {
    backgroundColor: Color.t,
    color: Color.t,
    width: int,
    height: int,
    position: LayoutTypes.positionType,
    top: int,
    bottom: int,
    left: int,
    right: int,
};

let make = (
    ~backgroundColor: Color.t = Colors.black,
    ~color: Color.t = Colors.white,
    ~width=Encoding.cssUndefined,
    ~height=Encoding.cssUndefined,
    ~position=LayoutTypes.Relative,
    ~top=Encoding.cssUndefined,
    ~bottom=Encoding.cssUndefined,
    ~left=Encoding.cssUndefined,
    ~right=Encoding.cssUndefined,
    _unit: unit
) => {

    let ret: t = {
        backgroundColor,
        color,
        width,
        height,
        position,
        top,
        bottom,
        left,
        right,
    };

    ret;
};

let defaultStyle = make( () );

let toLayoutNode = (s: t) => {
    let ret: LayoutTypes.cssStyle = {
                ...LayoutSupport.defaultStyle,
                position: s.position,
                top: s.top,
                left: s.left,
                bottom: s.bottom,
                right: s.right,
                width: s.width,
                height: s.height
            };
    ret;
};
