open Layout;

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
    ~width: Encoding.cssUndefined,
    ~height: Encoding.cssUndefined,
    ~position: LayoutTypes.positionType,
    ~top: Encoding.cssUndefined,
    ~bottom: Encoding.cssUndefined,
    ~left: Encoding.cssUndefined,
    ~right: Encoding.cssUndefined,
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
        width,
        height
    };

    ret;
};

let defaultStyle = make();

let toLayoutNode = (style: t) => {
    let ret = {...Layout.defaultStyle,
                position,
                top,
                left,
                bottom,
                right,
                width,
                height
            };
    ret;
};
