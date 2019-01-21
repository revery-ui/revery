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
      ~thumbLength=15,
      ~sliderLength=100,
      ~vertical=false,
      (),
    ) =>
  component(slots => {
    let (slideRef, _setSlideRef, slots) = React.Hooks.state(None, slots);
    let (thumbRef, _setThumbRef, slots) = React.Hooks.state(None, slots);
    let (isActive, setActive, slots) = React.Hooks.state(false, slots);
    let (thumbPosition, setThumbPosition, _slots: React.Hooks.empty) =
      React.Hooks.state(0, slots);

    let setSlideRef = r => {
      _setSlideRef(Some(r));
    };

    let setThumbRef = r => {
      _setThumbRef(Some(r));
    };

    let onMouseDown = (evt: NodeEvents.mouseButtonEventParams) => {
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

        let availableWidth = sliderWidth -. thumbWidth;

        let startPosition =
          vertical
            ? Vec2.get_y(sliderDimensions.min)
            : Vec2.get_x(sliderDimensions.min);
        let endPosition = startPosition +. availableWidth;

        let getValue = x =>
          if (x < startPosition) {
            startPosition;
          } else if (x > endPosition) {
            endPosition;
          } else {
            x;
          };

        let update = (mouseX, mouseY) => {
          let mousePosition = vertical ? mouseY : mouseX;
          let thumbPosition = getValue(mousePosition) -. startPosition;

          let normalizedValue =
            thumbPosition
            /. availableWidth
            *. (maximumValue -. minimumValue)
            +. minimumValue;
          setThumbPosition(int_of_float(thumbPosition));
          onValueChanged(normalizedValue);
        };

        update(evt.mouseX, evt.mouseY);
        setActive(true);

        Mouse.setCapture(
          ~onMouseMove=evt => update(evt.mouseX, evt.mouseY),
          ~onMouseUp=
            _evt => {
              Mouse.releaseCapture();
              setActive(false);
            },
          (),
        );
      | _ => ()
      };
    };

    let backgroundColor = Colors.darkGray;
    let thumbColor = Colors.gray;

    let opacity = isActive ? 1.0 : 0.8;

    let sliderHeight = 25;
    let trackHeight = 5;
    let thumbHeight = sliderHeight;
    let trackMargins = (sliderHeight - trackHeight) / 2;

    let style =
      Style.make(
        ~opacity,
        ~width={
          vertical ? sliderHeight : sliderLength;
        },
        ~height={
          vertical ? sliderLength : sliderHeight;
        },
        ~cursor=MouseCursors.pointer,
        (),
      );

    let thumbWidth = thumbLength;

    let trackStyle =
      Style.make(
        ~opacity,
        ~top={
          vertical ? 0 : trackMargins;
        },
        ~bottom={
          vertical ? 0 : trackMargins;
        },
        ~left={
          vertical ? trackMargins : 0;
        },
        ~right={
          vertical ? trackMargins : 0;
        },
        ~position=LayoutTypes.Absolute,
        ~backgroundColor,
        (),
      );

    <View onMouseDown style ref={r => setSlideRef(r)}>
      <View style=trackStyle />
      <View
        ref={r => setThumbRef(r)}
        style={Style.make(
          ~position=LayoutTypes.Absolute,
          ~height={vertical ? thumbWidth : thumbHeight},
          ~width={vertical ? thumbHeight : thumbWidth},
          ~left={vertical ? 0 : thumbPosition},
          ~top={vertical ? thumbPosition : 0},
          ~backgroundColor=thumbColor,
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
      ~vertical=false,
      (),
    ) =>
  React.element(
    make(~vertical, ~onValueChanged, ~minimumValue, ~maximumValue, ()),
  );
