/* open Revery_Core; */
open Revery_UI;
open Revery_UI.Transform;

let component = React.component("ScrollView");

let empty = React.listToElement([]);

let make =
    (~style, ~scrollLeft=0, ~scrollTop=0, children: React.syntheticElement) =>
  component(slots => {
    let (actualScrollTop, setScrollTop, slots) = React.Hooks.state(scrollTop, slots);
    let (actualScrollLeft, setScrollLeft, slots) = React.Hooks.state(scrollLeft, slots);

    let (outerRef: option(Revery_UI.node), setOuterRef, slots) = React.Hooks.state(None, slots);
    let (innerRef: option(Revery_UI.node), setInnerRef, _slots: React.Hooks.empty) =
      React.Hooks.state(None, slots);

    let scrollBarThickness = 10;

    let innerViewTransform = [
      TranslateX((-1.) *. float_of_int(actualScrollLeft)),
      TranslateY((-1.) *. float_of_int(actualScrollTop)),
    ];

    let (horizontalScrollBar, verticalScrollBar) = switch((outerRef, innerRef)) {
    | (Some(inner), Some(outer)) => {
        let childMeasurements = inner#measurements();
        let outerMeasurements = outer#measurements();

        let maxHeight = childMeasurements.height - outerMeasurements.height;
        let maxWidth = childMeasurements.width - outerMeasurements.width;

        prerr_endline ("CHILD HEIGHT: " ++ string_of_int(childMeasurements.height));
        prerr_endline ("CHILD WIDTH: " ++ string_of_int(childMeasurements.width));

        prerr_endline ("OUTER HEIGHT: " ++ string_of_int(outerMeasurements.height));
        prerr_endline ("OUTER WIDTH: " ++ string_of_int(outerMeasurements.width));


        let verticalThumbHeight = childMeasurements.height > 0 ? (outerMeasurements.height * outerMeasurements.height) / childMeasurements.height : 1;
        let horizontalThumbHeight = childMeasurements.width > 0 ? (outerMeasurements.width * outerMeasurements.width) / childMeasurements.width : 1;

        let isVerticalScrollbarVisible = maxHeight > -scrollBarThickness;
        let isHorizontalScrollbarVisible = maxWidth > -scrollBarThickness;


        prerr_endline ("MAX HEIGHT: " ++ string_of_int(maxHeight));
        prerr_endline ("MAX WIDTH: " ++ string_of_int(maxWidth));

        /* let isVerticalScrollBarVisible = childDimensions.height > outerDimensions.height; */

        let verticalScrollBar = isVerticalScrollbarVisible ?  <Slider onValueChanged={(v) => setScrollTop(-int_of_float(v))} minimumValue={0.} maximumValue={float_of_int(maxHeight)} sliderLength={outerMeasurements.height} thumbLength={verticalThumbHeight} trackThickness=scrollBarThickness thumbThickness=scrollBarThickness vertical={true} /> : empty;

        let horizontalScrollbar = isHorizontalScrollbarVisible ? <Slider onValueChanged={(v) => setScrollLeft(-int_of_float(v))} minimumValue={0.} maximumValue={float_of_int(maxWidth)} sliderLength={outerMeasurements.width} thumbLength={horizontalThumbHeight} trackThickness=scrollBarThickness thumbThickness=scrollBarThickness /> : empty;
        
        (horizontalScrollbar, verticalScrollBar); 
    }
    | _ => (empty, empty);
    };

    let outerStyle =Style.extend(
        ~position=LayoutTypes.Relative,
        ~overflow=LayoutTypes.Scroll,
        style,
        (),
    );

    let scroll = (wheelEvent: NodeEvents.mouseWheelEventParams) => {
        let newScrollTop = actualScrollTop - (int_of_float(wheelEvent.deltaY) * 25);
        setScrollTop(newScrollTop);
    };

/*     let sliderStyle = Style.make( */
/*         ~position=LayoutTypes.Absolute, */
/*         ~top=0, */
/*         ~bottom=0, */
/*         ~right=0, */
/*         (), */
/*     ); */

    <View 
        onMouseWheel={scroll}
        ref={r => setOuterRef(Some(r))} style={outerStyle}>
      <View
        ref={r => setInnerRef(Some(r))}
        style={Style.make(~transform=innerViewTransform, ~flexGrow=1, ())}>
        children
      </View>
      <View style={Style.make(~position=LayoutTypes.Absolute, ~bottom=0, ~left=0, ~right=0, ~height=scrollBarThickness, ())}>
      {horizontalScrollBar}
       </View>
      <View style={Style.make(~position=LayoutTypes.Absolute, ~right=0, ~top=0, ~bottom=0, ~width=scrollBarThickness, ())}>
      {verticalScrollBar}
       </View>
    </View>;
  });

let createElement = (~children, ~style, ~scrollLeft=0, ~scrollTop=0, ()) => {
  React.element(
    make(~style, ~scrollLeft, ~scrollTop, React.listToElement(children)),
  );
};
