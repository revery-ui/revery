open Revery_Core;
open Revery_UI;
open Revery_UI.Transform;
open Revery_UI_Primitives;

module Hooks = Revery_UI_Hooks;

type direction =
  | Top
  | Bottom;
type bouncingState =
  | Bouncing(direction, Animated.playback)
  | Idle;

let empty = React.empty;

let scrollTrackColor = Color.rgba(0.0, 0.0, 0.0, 0.4);
let scrollThumbColor = Color.rgba(0.5, 0.5, 0.5, 0.4);
let defaultBounce = Environment.os === Mac ? true : false;

type action =
  | ScrollUpdated(int);

let reducer = (action, _state) => {
  switch (action) {
  | ScrollUpdated(v) => v
  };
};

let%component make =
    (
      ~style,
      ~scrollLeft=0,
      ~scrollTop=0,
      ~bounce=defaultBounce,
      children=React.empty,
      ()
    ) => {
    let%hook (actualScrollTop, dispatch) =
      Hooks.reducer(~initialState=scrollTop, reducer);
    let%hook (outerRef: option(Revery_UI.node), setOuterRef) =
      Hooks.state(None);
    let%hook (actualScrollLeft, setScrollLeft) =
      Hooks.state(scrollLeft);
    let%hook (bouncingState, setBouncingState) = Hooks.state(Idle);

    let scrollBarThickness = 10;

    let innerViewTransform = [
      TranslateX((-1.) *. float_of_int(actualScrollLeft)),
      TranslateY((-1.) *. float_of_int(actualScrollTop)),
    ];

    let (horizontalScrollBar, verticalScrollBar, scroll) =
      switch (outerRef) {
      | Some(outer) =>
        let inner = outer#firstChild();
        let childMeasurements = inner#measurements();
        let outerMeasurements = outer#measurements();

        let maxHeight =
          max(0, childMeasurements.height - outerMeasurements.height);
        let maxWidth = childMeasurements.width - outerMeasurements.width;

        /*
         * TODO: #287
         * prerr_endline ("Child width: " ++ string_of_int(childMeasurements.width));
         * prerr_endline ("Container width: " ++ string_of_int(outerMeasurements.width));
         * This can be removed once #287 is fixed
         */

        let verticalThumbHeight =
          childMeasurements.height > 0
            ? outerMeasurements.height
              * outerMeasurements.height
              / childMeasurements.height
            : 1;
        let horizontalThumbHeight =
          childMeasurements.width > 0
            ? outerMeasurements.width
              * outerMeasurements.width
              / childMeasurements.width
            : 1;

        let isVerticalScrollbarVisible = maxHeight > 0;
        let isHorizontalScrollbarVisible = maxWidth > 0;

        let verticalScrollBar =
          isVerticalScrollbarVisible
            ? <Slider
                onValueChanged={v =>
                  dispatch(ScrollUpdated(int_of_float(v)))
                }
                minimumValue=0.
                maximumValue={float_of_int(maxHeight)}
                sliderLength={outerMeasurements.height}
                thumbLength=verticalThumbHeight
                value={float_of_int(actualScrollTop)}
                trackThickness=scrollBarThickness
                thumbThickness=scrollBarThickness
                minimumTrackColor=scrollTrackColor
                maximumTrackColor=scrollTrackColor
                thumbColor=scrollThumbColor
                vertical=true
              />
            : empty;

        /* TODO: #287
         * Need to investigate why the child width is not being reported (expanded) correctly.
         * Currently, the child width is clamped to the parent.
         * Is this a bug in flex?
         * Or something we need to fix in our styling?
         */
        let horizontalScrollbar =
          isHorizontalScrollbarVisible
            ? <Slider
                onValueChanged={v => setScrollLeft(_prevValue => - int_of_float(v))}
                minimumValue=0.
                maximumValue={float_of_int(maxWidth)}
                sliderLength={outerMeasurements.width}
                thumbLength=horizontalThumbHeight
                trackThickness=scrollBarThickness
                thumbThickness=scrollBarThickness
                minimumTrackColor=scrollTrackColor
                maximumTrackColor=scrollTrackColor
                thumbColor=scrollThumbColor
              />
            : empty;

        let scroll = (wheelEvent: NodeEvents.mouseWheelEventParams) => {
          let newScrollTop =
            actualScrollTop - int_of_float(wheelEvent.deltaY *. 25.);

          let isAtTop = newScrollTop < 0;
          let isAtBottom = newScrollTop > maxHeight;

          switch (bouncingState) {
          | Bouncing(Top, playback) when wheelEvent.deltaY < 0. =>
            playback.stop();
            setBouncingState(_prevState => Idle);
          | Bouncing(Bottom, playback) when wheelEvent.deltaY > 0. =>
            playback.stop();
            setBouncingState(_prevState => Idle);
          | Bouncing(_) => ()
          | Idle when !bounce && (isAtTop || isAtBottom) =>
            let clampedScrollTop = isAtTop ? 0 : maxHeight;
            dispatch(ScrollUpdated(clampedScrollTop));
          | Idle when bounce && (isAtTop || isAtBottom) =>
            open Animated;
            let direction = isAtTop ? Top : Bottom;
            let bounceAwayAnim = {
              toValue: float_of_int(newScrollTop),
              duration: Milliseconds(100.),
              delay: Seconds(0.),
              repeat: false,
              easing: Easing.cubicBezier(0.23, 1., 0.32, 1.),
              direction: `Normal,
            };
            let bounceBackAnim = {
              toValue: isAtTop ? 0. : float_of_int(maxHeight),
              duration: Milliseconds(800.),
              delay: Seconds(0.),
              repeat: false,
              easing: Easing.cubicBezier(0.23, 1., 0.32, 1.),
              direction: `Normal,
            };
            let playback =
              tween(
                floatValue(float_of_int(actualScrollTop)),
                bounceAwayAnim,
              )
              |> Chain.make
              |> Chain.add(
                   tween(
                     floatValue(float_of_int(newScrollTop)),
                     bounceBackAnim,
                   ),
                 )
              |> Chain.start(~update=v =>
                   dispatch(ScrollUpdated(int_of_float(v)))
                 );
            setBouncingState(_prevState => Bouncing(direction, playback));
          | Idle => dispatch(ScrollUpdated(newScrollTop))
        };
      };

      (horizontalScrollbar, verticalScrollBar, scroll);
    | _ => (empty, empty, (_ => ()))
    };

  let innerStyle =
    Style.[
      transform(innerViewTransform),
      position(`Absolute),
      top(0),
      /* TODO: #287 This styling will need to be adjusted to handle horizontal scrolling */
      left(0),
      right(verticalScrollBar == empty ? 0 : scrollBarThickness),
    ];

  let verticalScrollbarContainerStyle =
    Style.[
      position(`Absolute),
      right(0),
      top(0),
      bottom(0),
      width(scrollBarThickness),
    ];

  let horizontalScrollbarContainerStyle =
    Style.[
      position(`Absolute),
      right(0),
      left(0),
      bottom(0),
      height(scrollBarThickness),
    ];

  <View style>
    <View
      onMouseWheel=scroll
      ref={r => setOuterRef(_ => Some(r))}
      style=Style.[flexGrow(1), position(`Relative), overflow(`Scroll)]>
      <View style=innerStyle> children </View>
      <View style=verticalScrollbarContainerStyle> verticalScrollBar </View>
      <View style=horizontalScrollbarContainerStyle>
        horizontalScrollBar
      </View>
    </View>
  </View>;
};
