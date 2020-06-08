open Revery_UI;
open Revery_Core;
open Revery_UI_Primitives;
open Revery_Font;

module Hooks = Revery_UI_Hooks;

let noop = () => ();

let%component make =
              (
                ~text,
                ~inactiveStyle,
                ~activeStyle,
                ~fontSize=14.,
                ~fontFamily=Family.default,
                ~fontWeight=Weight.Normal,
                ~italic=false,
                ~monospaced=false,
                ~underlined=false,
                ~onClick=noop,
                (),
              ) => {
  let%hook (isHovered, onMouseEnter, onMouseLeave) = Hooks.hover();

  let outerStyle = Style.[cursor(MouseCursors.pointer)];

  <Clickable style=outerStyle onClick onMouseEnter onMouseLeave>
    <Text
      text
      fontFamily
      fontSize
      fontWeight
      italic
      monospaced
      style={isHovered ? activeStyle : inactiveStyle}
      underlined
    />
  </Clickable>;
};
