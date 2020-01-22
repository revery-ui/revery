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

let%component make =
              (
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
              ) => {
  let%hook (slideRef, setSlideRefOption) = Hooks.state(None);
  let%hook (thumbRef, setThumbRefOption) = Hooks.state(None);
  let%hook (isActive, setActive) = Hooks.state(false);
  /* Initial value is used to detect if the 'value' parameter ever changes */
  let%hook (initialValue, setInitialValue) = Hooks.state(value);
  let%hook (v, setV) = Hooks.state(value);

  /*
   * If the slider value is updated (controlled),
   * it should override whatever previous value was set
   */
  let v =
    if (value != initialValue) {
      setInitialValue(_ => value);
      setV(_ => value);
      value;
    } else {
      v;
    };

  let setSlideRef = r => setSlideRefOption(_ => Some(r));

  let setThumbRef = r => setThumbRefOption(_ => Some(r));

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
      clamp(
        mousePosition -. float(thumbLength) /. 2.,
        startPosition,
        endPosition,
      )
      -. startPosition;

    let normalizedValue =
      thumbPosition /. w *. (maximumValue -. minimumValue) +. minimumValue;
    setV(_ => normalizedValue);
    onValueChanged(normalizedValue);
  };

  let sliderComplete = () => {
    setActive(_ => false);
  };

  let%hook () =
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
      setActive(_ => true);
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
      int_of_float((v -. minimumValue) /. (maximumValue -. minimumValue) *. w)
      - thumbLength
      / 2
    | None => 0
    };

  let style =
    Style.[
      width(vertical ? sliderHeight : sliderLength),
      height(vertical ? sliderLength : sliderHeight),
      cursor(MouseCursors.pointer),
    ];

  let thumbWidth = thumbLength;

  let beforeTrackStyle =
    Style.[
      top(vertical ? 0 : trackMargins),
      bottom(
        vertical
          ? sliderLength - thumbPosition - thumbWidth / 2 : trackMargins,
      ),
      left(vertical ? trackMargins : 0),
      right(
        vertical
          ? trackMargins : sliderLength - thumbPosition - thumbWidth / 2,
      ),
      position(`Absolute),
      backgroundColor(minimumTrackColor),
    ];

  let afterTrackStyle =
    Style.[
      top(vertical ? thumbPosition + thumbWidth * 3 / 2 : trackMargins),
      bottom(vertical ? 0 : trackMargins),
      left(vertical ? trackMargins : thumbPosition + thumbWidth * 3 / 2),
      right(vertical ? trackMargins : 0),
      position(`Absolute),
      backgroundColor(sliderBackgroundColor),
    ];

  <Opacity opacity=sliderOpacity>
    <View onMouseDown style ref=setSlideRef>
      <View style=beforeTrackStyle />
      <View
        ref=setThumbRef
        style=Style.[
          position(`Absolute),
          height(vertical ? thumbWidth : thumbHeight),
          width(vertical ? thumbHeight : thumbWidth),
          left(vertical ? 0 : thumbPosition + thumbWidth / 2),
          top(vertical ? thumbPosition + thumbWidth / 2 : 0),
          backgroundColor(thumbColor),
        ]
      />
      <View style=afterTrackStyle />
    </View>
  </Opacity>;
};
