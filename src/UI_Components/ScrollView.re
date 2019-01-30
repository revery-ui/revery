open Revery_Core;
open Revery_UI;
open Revery_UI.Transform;

let component = React.component("ScrollView");

let empty = React.listToElement([]);

let scrollTrackColor = Color.rgba(0.0, 0.0, 0.0, 0.4);
let scrollThumbColor = Color.rgba(0.5, 0.5, 0.5, 0.4);

let make =
    (~style, ~scrollLeft=0, ~scrollTop=0, children: React.syntheticElement) =>
  component(slots => {
    let (actualScrollTop, setScrollTop, slots) =
      React.Hooks.state(scrollTop, slots);
    let (outerRef: option(Revery_UI.node), setOuterRef, slots) =
      React.Hooks.state(None, slots);
    let (actualScrollLeft, setScrollLeft, _slots: React.Hooks.empty) =
      React.Hooks.state(scrollLeft, slots);

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

        let maxHeight = childMeasurements.height - outerMeasurements.height;
        let maxWidth = childMeasurements.width - outerMeasurements.width;

        prerr_endline ("Child width: " ++ string_of_int(childMeasurements.width));
        prerr_endline ("Container width: " ++ string_of_int(outerMeasurements.width));

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
                onValueChanged={v => setScrollTop(int_of_float(v))}
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

        /* TODO:
         * Need to investigate why the child width is not being reported (expanded) correctly.
         * Currently, the child width is clamped to the parent.
         * Is this a bug in flex?
         * Or something we need to fix in our styling?
         */
        let horizontalScrollbar =
          isHorizontalScrollbarVisible
            ? <Slider
                onValueChanged={v => setScrollLeft(- int_of_float(v))}
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
            actualScrollTop - int_of_float(wheelEvent.deltaY) * 25;

          let clampedScrollTop =
            if (newScrollTop < 0) {
              0;
            } else if (newScrollTop > maxHeight) {
              maxHeight;
            } else {
              newScrollTop;
            };

          setScrollTop(clampedScrollTop);
        };

        (horizontalScrollbar, verticalScrollBar, scroll);
      | _ => (empty, empty, (_ => ()))
      };

    let innerStyle =
      Style.[
        transform(innerViewTransform),
        position(`Absolute),
        top(0),
        /* TODO: This styling will need to be adjusted to handle horizontal scrolling */
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
        ref={r => setOuterRef(Some(r))}
        style=Style.[flexGrow(1), position(`Relative), overflow(LayoutTypes.Scroll)]>
        <View style=innerStyle> children </View>
        <View style=verticalScrollbarContainerStyle> verticalScrollBar </View>
        <View style=horizontalScrollbarContainerStyle> horizontalScrollBar </View>
       </View>
    </View>;
  });

let createElement = (~children, ~style, ~scrollLeft=0, ~scrollTop=0, ()) => {
  React.element(
    make(~style, ~scrollLeft, ~scrollTop, React.listToElement(children)),
  );
};
