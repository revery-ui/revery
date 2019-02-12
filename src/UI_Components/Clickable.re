/*
 * Clickable.re
 *
 * This module provides a `<Clickable />` component, loosely inspired by the
 * `<Touchable />` component in React-Native, but geared towards the desktop.
 */

open Revery_UI;
open Revery_Core;
open Revery_Math;

type clickFunction = unit => unit;
let noop = () => ();

let isMouseInsideRef = (ref: node, mouseX: float, mouseY: float) => {
  let clickableDimensions: BoundingBox2d.t = ref#getBoundingBox();
  let pointVec = Vec2.create(mouseX, mouseY);
  BoundingBox2d.isPointInside(clickableDimensions, pointVec);
};

let component = React.component("Clickable");

let make =
    (
      ~style,
      ~onClick: clickFunction=noop,
      ~onBlur=?,
      ~onFocus=?,
      ~tabindex=?,
      ~onKeyDown=?,
      ~onKeyUp=?,
      ~onKeyPress=?,
      children: React.syntheticElement,
    ) =>
  component(slots => {
    let (clickableRef, setClickableRefOption, slots) =
      React.Hooks.state(None, slots);
    let setClickableRef = r => setClickableRefOption(Some(r));

    let (animatedOpacity, setOpacity, slots) =
      React.Hooks.state(0.8, slots);

    let onMouseMove = (mouseX: float, mouseY: float) => {
      switch (clickableRef) {
      | Some(clickable) =>
        if (isMouseInsideRef(clickable, mouseX, mouseY)) {
          setOpacity(1.0);
        } else {
          setOpacity(0.8);
        }
      | None => ()
      };
    };

    let onMouseUp = (mouseX: float, mouseY: float) => {
      switch (clickableRef) {
      | Some(clickable) =>
        if (isMouseInsideRef(clickable, mouseX, mouseY)) {
          onClick();
        }
      | None => ()
      };

      setOpacity(0.8);
      Mouse.releaseCapture();
    };

    let onMouseDown = _ => {
      Mouse.setCapture(
        ~onMouseMove=evt => onMouseMove(evt.mouseX, evt.mouseY),
        ~onMouseUp=evt => onMouseUp(evt.mouseX, evt.mouseY),
        (),
      );

      setOpacity(1.0);
    };

    let mergedStyles =
      Style.(
        merge(
          ~source=style,
          ~target=[opacity(animatedOpacity), cursor(MouseCursors.pointer)],
        )
      );

    (slots, <View
      style=mergedStyles
      onMouseDown
      ?onBlur
      ?onFocus
      ?onKeyDown
      ?onKeyUp
      ?onKeyPress
      tabindex
      ref={r => setClickableRef(r)}>
      children
    </View>);
  });

let createElement =
    (
      ~style=[],
      ~onClick: clickFunction=noop,
      ~onBlur=?,
      ~onFocus=?,
      ~tabindex=0,
      ~children,
      ~onKeyDown=?,
      ~onKeyUp=?,
      ~onKeyPress=?,
      (),
    ) =>
  make(
    ~style,
    ~onClick,
    ~onBlur?,
    ~onFocus?,
    ~onKeyDown?,
    ~onKeyUp?,
    ~onKeyPress?,
    ~tabindex,
    React.listToElement(children),
  );
