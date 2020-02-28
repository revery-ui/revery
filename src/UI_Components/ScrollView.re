open Revery_Core;
open Revery_UI;
open Revery_UI.Transform;
open Revery_UI_Primitives;

module Hooks = Revery_UI_Hooks;

module Log = (val Log.withNamespace("Revery.ScrollView"));

type bouncingState =
  | Bouncing(int)
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

let%component make =
              (
                ~style,
                ~scrollLeft=0,
                ~scrollTop=0,
                ~bounce=defaultBounce,
                ~children=React.empty,
                (),
              ) => {
  let%hook (actualScrollTop, dispatch) =
    Hooks.reducer(~initialState=scrollTop, reducer);
  let%hook (outerRef: option(Revery_UI.node), setOuterRef) =
    Hooks.state(None);
  let%hook (actualScrollLeft, setScrollLeft) = Hooks.state(scrollLeft);
  let%hook (bouncingState, setBouncingState) = Hooks.state(Idle);

  //let%hook (scrollview, setScrollview) = Hooks.state(() => Libscroll.scrollview_new());
  //setScrollview(_ => scrollview);
  let%hook (scrollViewRef) = Hooks.ref(None);
  let%hook () = Hooks.effect(OnMount, () => {
    let scrollView = Libscroll.scrollview_new();
    scrollViewRef := Some(scrollView);

    let dispose = () => {
       scrollViewRef := None;
    };   
    Some(dispose);
 });

  let%hook (actualScrollTop, _bounceAnimationState, resetBouncingAnimation) =
    switch (bouncingState) {
    | Idle =>
      // TODO: Why isn't Animation.const always sufficient to stop the timer?
      Hooks.animation(~active=false, Animation.const(actualScrollTop))

    | Bouncing(force) =>
      Hooks.animation(
        bounceAnimation(~origin=actualScrollTop, ~force), ~onComplete=() =>
        setBouncingState(_ => Idle)
      )
    };
  let setBouncingState = state => {
    resetBouncingAnimation();
    setBouncingState(state);
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
              onValueChanged={v => dispatch(ScrollUpdated(int_of_float(v)))}
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
              onValueChanged={v => setScrollLeft(_ => - int_of_float(v))}
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
              
      /*let pan = (panEvent: NodeEvents.panEventParams) => {
        switch (scrollViewRef^) {
        | None => ()
        | Some(scrollview) => {
          let timestamp = wheelEvent.timestamp;
          let delta = wheelEvent.delta;
          let axis = wheelEvent.axis;
        }
      }*/

      let scroll = (wheelEvent: NodeEvents.mouseWheelEventParams) => {
        switch (scrollViewRef^) {
        | Some(scrollview) => {//Libscroll.push_pan(scrollview, Libscroll.Axis.Vertical, 10.0, 0)
            Log.info("Scrollview existed");
            ()
        }
        | None => ()
        }
        let delta = int_of_float(wheelEvent.deltaY *. 25.);
        let newScrollTop = actualScrollTop - delta;

        let isAtTop = newScrollTop < 0;
        let isAtBottom = newScrollTop > maxHeight;

        switch (bouncingState) {
        | Bouncing(force) when force < 0 && wheelEvent.deltaY < 0. =>
          setBouncingState(_ => Idle)
        | Bouncing(force) when force > 0 && wheelEvent.deltaY > 0. =>
          setBouncingState(_ => Idle)
        | Bouncing(_) => ()
        | Idle when !bounce && (isAtTop || isAtBottom) =>
          let clampedScrollTop = isAtTop ? 0 : maxHeight;
          dispatch(ScrollUpdated(clampedScrollTop));
        | Idle when bounce && (isAtTop || isAtBottom) =>
          setBouncingState(_ => Bouncing(- delta * 2));
          dispatch(ScrollUpdated(isAtTop ? 0 : maxHeight));
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
