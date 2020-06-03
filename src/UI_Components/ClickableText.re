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
                ~italicized=false,
                ~monospaced=false,
                ~onClick=noop,
                (),
              ) => {
  let%hook (isHovered, onMouseEnter, onMouseLeave) = Hooks.hover();

  let outerStyle = Style.[cursor(MouseCursors.pointer)];
  let onMouseUp = _ => onClick();

  <View style=outerStyle onMouseUp onMouseEnter onMouseLeave>
    <Text
      text
      fontFamily
      fontSize
      fontWeight
      italicized
      monospaced
      style={isHovered ? activeStyle : inactiveStyle}
    />
  </View>;
};
