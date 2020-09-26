open Revery_Core;
open Revery_UI;
open Revery_UI.Transform;
open Revery_UI_Primitives;

module Hooks = Revery_UI_Hooks;

type bouncingState =
  | Bouncing(int)
  | Idle;

let empty = React.empty;

let scrollTrackColor = Color.rgba(0.0, 0.0, 0.0, 0.4);
let scrollThumbColor = Color.rgba(0.5, 0.5, 0.5, 0.4);
let isMac = Environment.os === Mac;

type action =
  | ScrollUpdated(int);

let reducer = (action, _state) => {
  switch (action) {
  | ScrollUpdated(v) => v
  };
};

let bounceAnimation = (~origin, ~force) =>
  Animation.(
    {
      let bounceAway =
        animate(Time.ms(100))
        |> ease(Easing.cubicBezier(0.23, 1., 0.32, 1.))
        |> tween(float(origin), float(origin + force));

      let bounceBack =
        Animation.animate(Time.ms(800))
        |> ease(Easing.cubicBezier(0.23, 1., 0.32, 1.))
        |> tween(float(origin + force), float(origin));

      bounceAway |> andThen(~next=bounceBack) |> map(int_of_float);
    }
  );

let bounceAnimationHook = (scrollPosition, bouncingState, setBouncingState) => {
  switch (bouncingState) {
  | Idle =>
    // TODO: Why isn't Animation.const always sufficient to stop the timer?
    Hooks.animation(~active=false, Animation.const(scrollPosition))

  | Bouncing(force) =>
    Hooks.animation(
      bounceAnimation(~origin=scrollPosition, ~force), ~onComplete=() =>
      setBouncingState(_ => Idle)
    )
  };
};

let handeScroll =
    (
      ~deltaValue,
      ~bounce,
      ~scrollPosition,
      ~maxScrollValue,
      ~bouncingState,
      ~setBouncingState,
      ~scrollDispatch,
    ) => {
  let delta = int_of_float(deltaValue *. 25.);
  let newScrollPosition = scrollPosition - delta;

  let isMinScrollValue = newScrollPosition < 0;
  let isMaxScrollValue = newScrollPosition > maxScrollValue;

  switch (bouncingState) {
  | Bouncing(force) when force < 0 && deltaValue < 0. =>
    setBouncingState(_ => Idle)
  | Bouncing(force) when force > 0 && deltaValue > 0. =>
    setBouncingState(_ => Idle)
  | Bouncing(_) => ()
  | Idle when !bounce && (isMinScrollValue || isMaxScrollValue) =>
    let clampedScrollValue = isMinScrollValue ? 0 : maxScrollValue;
    scrollDispatch(ScrollUpdated(clampedScrollValue));
  | Idle when bounce && (isMinScrollValue || isMaxScrollValue) =>
    setBouncingState(_ => Bouncing(- delta * 2));
    scrollDispatch(ScrollUpdated(isMinScrollValue ? 0 : maxScrollValue));
  | Idle => scrollDispatch(ScrollUpdated(newScrollPosition))
  };
};

let%component make =
              (
                ~style,
                ~scrollLeft=0,
                ~scrollTop=0,
                ~bounce=isMac,
                ~children=React.empty,
                (),
              ) => {
  let%hook (outerRef: option(Revery_UI.node), setOuterRef) =
    Hooks.state(None);

  let%hook (actualScrollTop, scrollYdispatch) =
    Hooks.reducer(~initialState=scrollTop, reducer);
  let%hook (bouncingStateY, setBouncingStateY) = Hooks.state(Idle);
  let%hook (actualScrollTop, _bounceAnimationState, resetBouncingAnimationY) =
    bounceAnimationHook(actualScrollTop, bouncingStateY, setBouncingStateY);

  let setBouncingStateY = state => {
    resetBouncingAnimationY();
    setBouncingStateY(state);
  };

  let%hook (actualScrollLeft, scrollXdispatch) =
    Hooks.reducer(~initialState=scrollLeft, reducer);
  let%hook (bouncingStateX, setBouncingStateX) = Hooks.state(Idle);
  let%hook (actualScrollLeft, _bounceAnimationState, resetBouncingAnimationX) =
    bounceAnimationHook(actualScrollLeft, bouncingStateX, setBouncingStateX);
  let setBouncingStateX = state => {
    resetBouncingAnimationX();
    setBouncingStateX(state);
  };

  let scrollBarThickness = 10;

  let innerViewTransform = [
    TranslateX((-1.) *. float_of_int(actualScrollLeft)),
    TranslateY((-1.) *. float_of_int(actualScrollTop)),
  ];

  let (horizontalScrollBar, verticalScrollBar, scroll) =
    switch (outerRef) {
    | Some(outer) =>
      let inner = outer#firstChild();

      // TODO: #287 For some reason `inner` component doesn't get expanded by it's childrens
      // Could be bug with https://github.com/jordwalke/flex or how we use it

      let maxChildWidth =
        List.fold_left(
          (maxWidth, child: node) => {
            let width = child#measurements().width;

            if (maxWidth > width) {
              maxWidth;
            } else {
              width;
            };
          },
          0,
          inner#getChildren(),
        );

      let childHeight = inner#measurements().height;
      let outerMeasurements = outer#measurements();

      let maxHeight = max(0, childHeight - outerMeasurements.height);
      let maxWidth = max(0, maxChildWidth - outerMeasurements.width);

      /*
       * TODO: #287
       * prerr_endline ("Child width: " ++ string_of_int(childMeasurements.width));
       * prerr_endline ("Container width: " ++ string_of_int(outerMeasurements.width));
       * This can be removed once #287 is fixed
       */

      let verticalThumbHeight =
        childHeight > 0
          ? outerMeasurements.height * outerMeasurements.height / childHeight
          : 1;
      let horizontalThumbHeight =
        maxChildWidth > 0
          ? outerMeasurements.width * outerMeasurements.width / maxChildWidth
          : 1;

      let isVerticalScrollBarVisible = maxHeight > 0;
      let isHorizontalScrollBarVisible = maxWidth > 0;

      let verticalScrollBar =
        isVerticalScrollBarVisible
          ? <Slider
              onValueChanged={v =>
                scrollYdispatch(ScrollUpdated(int_of_float(v)))
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

      let horizontalScrollBar =
        isHorizontalScrollBarVisible
          ? <Slider
              onValueChanged={v =>
                scrollXdispatch(ScrollUpdated(int_of_float(v)))
              }
              minimumValue=0.
              maximumValue={float_of_int(maxWidth)}
              sliderLength={outerMeasurements.width}
              thumbLength=horizontalThumbHeight
              value={float_of_int(actualScrollLeft)}
              trackThickness=scrollBarThickness
              thumbThickness=scrollBarThickness
              minimumTrackColor=scrollTrackColor
              maximumTrackColor=scrollTrackColor
              thumbColor=scrollThumbColor
            />
          : empty;

      let scroll = (wheelEvent: NodeEvents.mouseWheelEventParams) => {
        let horizontalScroll =
          wheelEvent.shiftKey || abs_float(wheelEvent.deltaX) > 0.;

        if (horizontalScroll) {
          if (isHorizontalScrollBarVisible) {
            let horizontalScrollMultiplier = isMac ? (-1.) : 1.;
            handeScroll(
              ~deltaValue=
                abs_float(wheelEvent.deltaX) > 0.
                  ? horizontalScrollMultiplier *. wheelEvent.deltaX
                  : wheelEvent.deltaY,
              ~bounce,
              ~scrollPosition=actualScrollLeft,
              ~maxScrollValue=maxWidth,
              ~bouncingState=bouncingStateX,
              ~setBouncingState=setBouncingStateX,
              ~scrollDispatch=scrollXdispatch,
            );
          };
        } else if (isVerticalScrollBarVisible) {
          handeScroll(
            ~deltaValue=wheelEvent.deltaY,
            ~bounce,
            ~scrollPosition=actualScrollTop,
            ~maxScrollValue=maxHeight,
            ~bouncingState=bouncingStateY,
            ~setBouncingState=setBouncingStateY,
            ~scrollDispatch=scrollYdispatch,
          );
        };
      };
      (horizontalScrollBar, verticalScrollBar, scroll);
    | _ => (empty, empty, (_ => ()))
    };

  let innerStyle =
    Style.[
      transform(innerViewTransform),
      position(`Absolute),
      top(0),
      left(horizontalScrollBar == empty ? 0 : scrollBarThickness),
      right(verticalScrollBar == empty ? 0 : scrollBarThickness),
    ];

  let verticalScrollBarContainerStyle =
    Style.[
      position(`Absolute),
      right(0),
      top(0),
      bottom(0),
      width(scrollBarThickness),
    ];

  let horizontalScrollBarContainerStyle =
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
      <View style=verticalScrollBarContainerStyle> verticalScrollBar </View>
      <View style=horizontalScrollBarContainerStyle>
        horizontalScrollBar
      </View>
    </View>
  </View>;
};
