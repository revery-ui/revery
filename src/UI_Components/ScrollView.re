open Revery_Core;
open Revery_UI;
open Revery_UI.Transform;

let component = React.component("ScrollView");

let empty = React.listToElement([]);

let make =
    (~style, ~scrollLeft=0, ~scrollTop=0, children: React.syntheticElement) =>
  component(slots => {
    let (actualScrollTop, setScrollTop, slots) = React.Hooks.state(scrollTop, slots);

    let (outerRef: option(Revery_UI.node), setOuterRef, slots) = React.Hooks.state(None, slots);
    let (innerRef: option(Revery_UI.node), setInnerRef, _slots: React.Hooks.empty) =
      React.Hooks.state(None, slots);

    let innerViewTransform = [
      TranslateX((-1.) *. float_of_int(scrollLeft)),
      TranslateY((-1.) *. float_of_int(actualScrollTop)),
    ];

    let (horizontalScrollVisible, verticalScrollVisible) = switch((outerRef, innerRef)) {
    | (Some(_inner), Some(_outer)) => {
        /* let childMeasurements = inner#measurements(); */
        /* let outerMeasurements = outer#measurements(); */

        /* let isVerticalScrollBarVisible = childDimensions.height > outerDimensions.height; */
        
        (true, true); 
    }
    | _ => (false, false);
    };

    let outerStyle =Style.extend(
        ~position=LayoutTypes.Relative,
        ~borderRight=Style.Border.make(~width={horizontalScrollVisible ? 2 : 0}, ~color=Colors.green, ()),
        ~borderBottom=Style.Border.make(~width={verticalScrollVisible ? 2 : 0}, ~color=Colors.green, ()),
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
        style={Style.make(~transform=innerViewTransform, ())}>
        children
      </View>
      <Slider onValueChanged={(v) => setScrollTop(int_of_float(v))} minimumValue={0.} maximumValue={100.} vertical={true} />
    </View>;
  });

let createElement = (~children, ~style, ~scrollLeft=0, ~scrollTop=0, ()) => {
  React.element(
    make(~style, ~scrollLeft, ~scrollTop, React.listToElement(children)),
  );
};
