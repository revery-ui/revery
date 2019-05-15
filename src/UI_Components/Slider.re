/*
 * Slider.re
 *
 * This module provides a `<Slider />` component, loosely inspired by the
 * `<Slider />` component in React-Native, but geared towards the desktop.
 */

open Revery_Math;
open Revery_Core;
open Revery_UI;
open Revery_UI_Primitives;

module Hooks = Revery_UI_Hooks;

type clickFunction = unit => unit;
let noop = () => ();

type valueChangedFunction = float => unit;
let noopValueChanged = _f => ();

let component = React.component("Slider");

let createElement =
    (
      ~children as _,
      ~onValueChanged=noopValueChanged,
      ~minimumValue=0.,
      ~maximumValue=1.,
      ~value=0.,
      ~vertical=false,
      ~thumbLength=15,
      ~sliderLength=100,
      ~thumbThickness=15,
      ~trackThickness=5,
      ~maximumTrackColor=Colors.darkGray,
      ~minimumTrackColor=Color.hex("#90f7ff"),
      ~thumbColor=Colors.gray,
      (),
    ) =>
  component(hooks => {
    let (slideRef, setSlideRefOption, hooks) = Hooks.state(None, hooks);
    let (thumbRef, setThumbRefOption, hooks) = Hooks.state(None, hooks);
    let (isActive, setActive, hooks) = Hooks.state(false, hooks);
    /* Initial value is used to detect if the 'value' parameter ever changes */
    let (initialValue, setInitialValue, hooks) = Hooks.state(value, hooks);
    let (v, setV, hooks) = Hooks.state(value, hooks);

    /*
     * If the slider value is updated (controlled),
     * it should override whatever previous value was set
     */
    let v =
      if (value != initialValue) {
        setInitialValue(value);
        setV(value);
        value;
      } else {
        v;
      };

    let setSlideRef = r => setSlideRefOption(Some(r));

    let setThumbRef = r => setThumbRefOption(Some(r));

    let availableWidth =
      switch (slideRef, thumbRef) {
      | (Some(slider), Some(thumb)) =>
        let sliderDimensions: BoundingBox2d.t = slider#getBoundingBox();
        let thumbDimensions: BoundingBox2d.t = thumb#getBoundingBox();

        let sliderWidth =
          vertical
            ? Vec2.get_y(sliderDimensions.max)
              -. Vec2.get_y(sliderDimensions.min)
            : Vec2.get_x(sliderDimensions.max)
              -. Vec2.get_x(sliderDimensions.min);

        let thumbWidth =
          vertical
            ? Vec2.get_y(thumbDimensions.max)
              -. Vec2.get_y(thumbDimensions.min)
            : Vec2.get_x(thumbDimensions.max)
              -. Vec2.get_x(thumbDimensions.min);

        Some(sliderWidth -. thumbWidth);
      | _ => None
      };

    let sliderUpdate = (w, startPosition, endPosition, mouseX, mouseY) => {
      let mousePosition = vertical ? mouseY : mouseX;
      let thumbPosition =
        clamp(mousePosition, startPosition, endPosition) -. startPosition;

      let normalizedValue =
        thumbPosition /. w *. (maximumValue -. minimumValue) +. minimumValue;
      setV(normalizedValue);
      onValueChanged(normalizedValue);
    };

    let sliderComplete = () => {
      setActive(false);
    };

    let hooks =
      Hooks.effect(
        Always,
        () => {
          let isCaptured = isActive;

          if (isCaptured) {
            switch (slideRef, availableWidth) {
            | (Some(slider), Some(w)) =>
              let sliderDimensions: BoundingBox2d.t = slider#getBoundingBox();

              let startPosition =
                vertical
                  ? Vec2.get_y(sliderDimensions.min)
                  : Vec2.get_x(sliderDimensions.min);
              let endPosition = startPosition +. w;

              Mouse.setCapture(
                ~onMouseMove=
                  evt =>
                    sliderUpdate(
                      w,
                      startPosition,
                      endPosition,
                      evt.mouseX,
                      evt.mouseY,
                    ),
                ~onMouseUp=_evt => sliderComplete(),
                (),
              );
            | _ => ()
            };
          };

          Some(
            () =>
              if (isCaptured) {
                Mouse.releaseCapture();
              },
          );
        },
        hooks,
      );

    let onMouseDown = (evt: NodeEvents.mouseButtonEventParams) =>
      switch (slideRef, availableWidth) {
      | (Some(slider), Some(w)) =>
        let sliderDimensions: BoundingBox2d.t = slider#getBoundingBox();

        let startPosition =
          vertical
            ? Vec2.get_y(sliderDimensions.min)
            : Vec2.get_x(sliderDimensions.min);
        let endPosition = startPosition +. w;

        sliderUpdate(w, startPosition, endPosition, evt.mouseX, evt.mouseY);
        setActive(true);
      | _ => ()
      };

    let sliderBackgroundColor = maximumTrackColor;

    let sliderOpacity = isActive ? 1.0 : 0.8;

    let sliderHeight = max(thumbThickness, trackThickness);
    let trackHeight = trackThickness;
    let thumbHeight = thumbThickness;
    let trackMargins = (sliderHeight - trackHeight) / 2;

    let thumbPosition =
      switch (availableWidth) {
      | Some(w) =>
        int_of_float(
          (v -. minimumValue) /. (maximumValue -. minimumValue) *. w,
        )
      | None => 0
      };

    let style =
      Style.[
        opacity(sliderOpacity),
        width(vertical ? sliderHeight : sliderLength),
        height(vertical ? sliderLength : sliderHeight),
        cursor(MouseCursors.pointer),
      ];

    let thumbWidth = thumbLength;

    let beforeTrackStyle =
      Style.[
        opacity(sliderOpacity),
        top(vertical ? 0 : trackMargins),
        bottom(vertical ? sliderLength - thumbPosition : trackMargins),
        left(vertical ? trackMargins : 0),
        right(vertical ? trackMargins : sliderLength - thumbPosition),
        position(`Absolute),
        backgroundColor(minimumTrackColor),
      ];

    let afterTrackStyle =
      Style.[
        opacity(sliderOpacity),
        top(vertical ? thumbPosition + thumbWidth : trackMargins),
        bottom(vertical ? 0 : trackMargins),
        left(vertical ? trackMargins : thumbPosition + thumbWidth),
        right(vertical ? trackMargins : 0),
        position(`Absolute),
        backgroundColor(sliderBackgroundColor),
      ];

    (
      hooks,
      <View onMouseDown style ref={r => setSlideRef(r)}>
        <View style=beforeTrackStyle />
        <View
          ref={r => setThumbRef(r)}
          style=Style.[
            position(`Absolute),
            height(vertical ? thumbWidth : thumbHeight),
            width(vertical ? thumbHeight : thumbWidth),
            left(vertical ? 0 : thumbPosition),
            top(vertical ? thumbPosition : 0),
            backgroundColor(thumbColor),
          ]
        />
        <View style=afterTrackStyle />
      </View>,
    );
  });
