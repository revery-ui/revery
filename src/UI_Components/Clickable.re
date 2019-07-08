/*
 * Clickable.re
 *
 * This module provides a `<Clickable />` component, loosely inspired by the
 * `<Touchable />` component in React-Native, but geared towards the desktop.
 */

open Revery_UI;
open Revery_Core;
open Revery_Math;
open Revery_UI_Primitives;

module Hooks = Revery_UI_Hooks;

type clickFunction = unit => unit;
type clickFunctionWithEvt = NodeEvents.mouseButtonEventParams => unit;
let noop = () => ();
let noopEvt = _evt => ();

let isMouseInsideRef = (ref: node, mouseX: float, mouseY: float) => {
  let clickableDimensions: BoundingBox2d.t = ref#getBoundingBoxClipped();
  let pointVec = Vec2.create(mouseX, mouseY);
  BoundingBox2d.isPointInside(clickableDimensions, pointVec);
};

let component = React.component("Clickable");

let make =
    (
      ~style,
      ~onClick: clickFunction=noop,
      ~onRightClick: clickFunction=noop,
      ~onAnyClick: clickFunctionWithEvt=noopEvt,
      ~componentRef=?,
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
      Hooks.state(None, slots);

    let setClickableRef = r => {
      switch (componentRef) {
      | Some(fn) => fn(r)
      | None => ()
      };
      setClickableRefOption(Some(r));
    };

    let (animatedOpacity, setOpacity, slots) = Hooks.state(0.8, slots);

    let onMouseMove = (mouseX: float, mouseY: float) =>
      switch (clickableRef) {
      | Some(clickable) =>
        if (isMouseInsideRef(clickable, mouseX, mouseY)) {
          setOpacity(1.0);
        } else {
          setOpacity(0.8);
        }
      | None => ()
      };

    let onMouseUp = (mouseEvt: NodeEvents.mouseButtonEventParams) => {
      switch (clickableRef) {
      | Some(clickable) =>
        if (isMouseInsideRef(clickable, mouseEvt.mouseX, mouseEvt.mouseY)) {
          switch (mouseEvt.button) {
          | MouseButton.BUTTON_LEFT => onClick()
          | MouseButton.BUTTON_RIGHT => onRightClick()
          | _ => ()
          };

          onAnyClick(mouseEvt);
        }
      | _ => ()
      };

      setOpacity(0.8);

      /* TODO Releasing capture in here means
         if multiple buttons are pressed simutaneously
         there would a race condition
         Not sure we need fix it though */
      Mouse.releaseCapture();
    };

    let onMouseDown = (mouseEvt: NodeEvents.mouseButtonEventParams) => {
      switch (mouseEvt.button) {
      | MouseButton.BUTTON_LEFT =>
        Mouse.setCapture(
          ~onMouseMove=evt => onMouseMove(evt.mouseX, evt.mouseY),
          ~onMouseUp=evt => onMouseUp(evt),
          (),
        );
        setOpacity(1.0);
      | _ => Mouse.setCapture(~onMouseUp=evt => onMouseUp(evt), ())
      };
    };

    let mergedStyles =
      Style.(
        merge(
          ~source=style,
          ~target=[opacity(animatedOpacity), cursor(MouseCursors.pointer)],
        )
      );

    (
      slots,
      <View
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
      </View>,
    );
  });

let createElement =
    (
      ~style=[],
      ~onClick: clickFunction=noop,
      ~onRightClick: clickFunction=noop,
      ~onAnyClick: clickFunctionWithEvt=noopEvt,
      ~onBlur=?,
      ~onFocus=?,
      ~tabindex=0,
      ~children,
      ~onKeyDown=?,
      ~onKeyUp=?,
      ~onKeyPress=?,
      ~componentRef=?,
      (),
    ) =>
  make(
    ~style,
    ~onClick,
    ~onRightClick,
    ~onAnyClick,
    ~onBlur?,
    ~onFocus?,
    ~onKeyDown?,
    ~onKeyUp?,
    ~onKeyPress?,
    ~tabindex,
    ~componentRef?,
    React.listToElement(children),
  );
