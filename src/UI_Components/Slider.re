/*
 * Slider.re
 *
 * This module provides a `<Slider />` component, loosely inspired by the
 * `<Slider />` component in React-Native, but geared towards the desktop.
 */

open Revery_Core;
open Revery_UI;

type clickFunction = unit => unit;
let noop = () => ();

type valueChangedFunction = float => unit;
let noopValueChanged = _f => ();

let component = React.component("Slider");

let make =
    (
      ~onValueChanged=noopValueChanged,
      ~minimumValue=0.,
      ~maximumValue=1.0,
      (),
    ) =>
  component(slots => {
    ignore((minimumValue, maximumValue));

    let (ref, _setRef, slots) = React.Hooks.state(None, slots);
    let (curOffset, _setOffset, _slots: React.Hooks.empty) =
      React.Hooks.state(0., slots);

    let setRef = r => {
      print_endline("SETTING REF: " ++ string_of_int(r#getInternalId()));
      _setRef(Some(r));
    };

    let onMouseDown = _evt => {
      print_endline("MOUSEDOWN");
      Mouse.setCapture(
        ~onMouseMove=
          evt => {
            print_endline("MOVING: " ++ string_of_float(evt.mouseX));

            switch (ref) {
            | Some(r) =>
              print_endline("got ref: " ++ string_of_int(r#getInternalId()))
            | None => print_endline("no ref")
            };

            /* TODO: Proper calculation */
            onValueChanged(evt.mouseX);
          },
        ~onMouseUp=_evt => Mouse.releaseCapture(),
        (),
      );
    };

    let backgroundColor =
      switch (ref) {
      | Some(_) => Colors.red
      | None => Colors.black
      };

    let style = Style.make(~width=100, ~height=25, ~backgroundColor, ());

    <View onMouseDown style ref={r => setRef(r)}>
      <View
        style={Style.make(
          ~position=LayoutTypes.Absolute,
          ~height=25,
          ~width=15,
          ~left=int_of_float(curOffset),
          ~top=0,
          ~backgroundColor=Colors.yellow,
          (),
        )}
      />
    </View>;
  });

let createElement =
    (
      ~children as _,
      ~onValueChanged=noopValueChanged,
      ~minimumValue=0.,
      ~maximumValue=1.,
      (),
    ) =>
  React.element(make(~onValueChanged, ~minimumValue, ~maximumValue, ()));
