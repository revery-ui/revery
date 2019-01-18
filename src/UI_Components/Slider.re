/*
 * Slider.re
 *
 * This module provides a `<Slider />` component, loosely inspired by the
 * `<Slider />` component in React-Native, but geared towards the desktop.
 */

open Revery_Math;
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
    let (slideRef, _setSlideRef, slots) = React.Hooks.state(None, slots);
    let (thumbRef, _setThumbRef, slots) = React.Hooks.state(None, slots);
    let (thumbPosition, setThumbPosition, _slots: React.Hooks.empty) = React.Hooks.state(0, slots);

    let setSlideRef = r => {
      _setSlideRef(Some(r));
    };

    let setThumbRef = r => {
     _setThumbRef(Some(r));   
    };

    let onMouseDown = (evt: NodeEvents.mouseButtonEventParams)  => {

      switch ((slideRef, thumbRef)) {
      | (Some(slider), Some(thumb)) => {
        let sliderDimensions: BoundingBox2d.t = slider#getBoundingBox();
          let thumbDimensions: BoundingBox2d.t = thumb#getBoundingBox();
        
          let sliderWidth = Vec2.get_x(sliderDimensions.max) -. Vec2.get_x(sliderDimensions.min);
          let thumbWidth = Vec2.get_x(thumbDimensions.max) -. Vec2.get_x(thumbDimensions.min);
          let availableWidth = sliderWidth -. thumbWidth;

          let startPosition = Vec2.get_x(sliderDimensions.min);
          let endPosition = startPosition +. availableWidth;

          let getValue = (x) => {
            if (x < startPosition) {
                startPosition
            } else if(x > endPosition) {
                endPosition
            } else {
                x
            }
          };

          let update = (mouseX) => {
            let thumbPosition = getValue(mouseX)  -. startPosition;

            let normalizedValue = ((thumbPosition /. availableWidth) *. (maximumValue -. minimumValue)) +. minimumValue;
            setThumbPosition(int_of_float(thumbPosition));
            onValueChanged(normalizedValue);
          };

          update(evt.mouseX);

          Mouse.setCapture(
            ~onMouseMove=(evt) => update(evt.mouseX),
            ~onMouseUp=_evt => Mouse.releaseCapture(),
            (),
          );
            
          }
      | _ => ()
      };

    };

    let backgroundColor =Colors.red;

    let style = Style.make(~width=100, ~height=25, ~backgroundColor, ());

    <View onMouseDown style ref={r => setSlideRef(r)}>
      <View
        ref={r => setThumbRef(r)}
        style={Style.make(
          ~position=LayoutTypes.Absolute,
          ~height=25,
          ~width=15,
          ~left=thumbPosition,
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
