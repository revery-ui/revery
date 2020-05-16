open Revery_UI;
open Revery_Core;
open Revery_UI_Primitives;

module Hooks = Revery_UI_Hooks;

let noop = () => ();

let%component make = (~text, ~inactiveStyle, ~activeStyle, ~onClick=noop, ()) => {
  let%hook (isHovered, setHovered) = Hooks.state(false);

  let outerStyle = Style.[cursor(MouseCursors.pointer)];
  let onMouseEnter = _ => setHovered(_ => true);
  let onMouseLeave = _ => setHovered(_ => false);
  let onMouseUp = _ => onClick();

  <View style=outerStyle onMouseUp onMouseEnter onMouseLeave>
    <Text text style={isHovered ? activeStyle : inactiveStyle} />
  </View>;
};
