/*
 * Clickable.re
 *
 * This module provides a `<Clickable />` component, loosely inspired by the
 * `<Touchable />` component in React-Native, but geared towards the desktop.
 */

open Revery_UI;
open Revery_Core;

type clickFunction = unit => unit;
let noop = () => ();

let component = React.component("Clickable");

let make =
    (
      ~style=Style.defaultStyle,
      ~onClick: clickFunction=noop,
      ~onBlur=?,
      ~onFocus=?,
      ~tabindex=?,
      children: React.syntheticElement,
    ) =>
  component(slots => {
    let (opacity, setOpacity, _slots: React.Hooks.empty) =
      React.Hooks.state(0.8, slots);

    /* TODO:
     *
     * This logic isn't really correct,
     * for the case where you hold down the mouse,
     * move around (leave the item), and come back.
     */
    let onMouseDown = _ => setOpacity(1.0);
    let onMouseUp = _ => {
      setOpacity(0.8);
      onClick();
    };

    let style2 =
      Style.extend(style, ~opacity, ~cursor=MouseCursors.pointer, ());

    <View style=style2 onMouseDown onMouseUp ?onBlur ?onFocus tabindex>
      children
    </View>;
  });

let createElement =
    (
      ~style=Style.defaultStyle,
      ~onClick: clickFunction=noop,
      ~onBlur=?,
      ~onFocus=?,
      ~tabindex=0,
      ~children,
      (),
    ) =>
  React.element(
    make(
      ~style,
      ~onClick,
      ~onBlur?,
      ~onFocus?,
      ~tabindex,
      React.listToElement(children),
    ),
  );
