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
      ~style,
      ~onClick: clickFunction=noop,
      ~onBlur=?,
      ~onFocus=?,
      ~tabindex=?,
      children: React.syntheticElement,
    ) =>
  component(slots => {
    let (animatedOpacity, setOpacity, _slots: React.Hooks.empty) =
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

    let mergedStyles =
      Style.(
        merge(
          ~source=style,
          ~target=[opacity(animatedOpacity), cursor(MouseCursors.pointer)],
        )
      );

    <View style=mergedStyles onMouseDown onMouseUp ?onBlur ?onFocus tabindex>
      children
    </View>;
  });

let createElement =
    (
      ~style=[],
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
