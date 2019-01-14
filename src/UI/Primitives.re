/*
 * Primitives
 *
 * These are the 'raw primitives' that our API describes.
 * These map directly to the raw nodes that get emitted in our scene graph.
 *
 * More details here:
 * https://github.com/bryphe/reason-reactify
 */

module View = {
    let component = UiReact.nativeComponent("view");

    let make = (
      ~onMouseDown=?,
      ~onMouseMove=?,
      ~onMouseUp=?,
      ~onMouseWheel=?,
      ~ref=?,
      ~style=Style.defaultStyle,
      children
    ) => {
       component((_: UiReact.Slots.empty) => {
        make: () => {
            let events = NodeEvents.make(~ref?, ~onMouseDown?, ~onMouseMove?, ~onMouseUp?, ~onMouseWheel?, ());
            let node =  (new ViewNode.viewNode)();
            node#setEvents(events);
            node#setStyle(style);
            node;
        },
        configureInstance: (~isFirstRender as _, node) => {
            let events = NodeEvents.make(~ref?, ~onMouseDown?, ~onMouseMove?, ~onMouseUp?, ~onMouseWheel?, ());
            node#setEvents(events);
            node#setStyle(style);
            node; 
        },
        children
       })
    };

    let createElement = (~onMouseDown=?, ~onMouseMove=?, ~onMouseUp=?, ~onMouseWheel=?, ~ref=?, ~style=Style.defaultStyle, ~children, ()) => {
        UiReact.element(make(~onMouseDown?, ~onMouseMove?, ~onMouseUp?, ~onMouseWheel?, ~ref?, ~style, UiReact.listToElement(children)));
    }
};

/* let view = */
/*     ( */
/*       ~onMouseDown=?, */
/*       ~onMouseMove=?, */
/*       ~onMouseUp=?, */
/*       ~onMouseWheel=?, */
/*       ~children, */
/*       ~ref=?, */
/*       ~style=Style.defaultStyle, */
/*       (), */
/*     ) => */
/*   UiReact.primitiveComponent( */
/*     View( */
/*       style, */
/*       NodeEvents.make( */
/*         ~ref?, */
/*         ~onMouseDown?, */
/*         ~onMouseMove?, */
/*         ~onMouseUp?, */
/*         ~onMouseWheel?, */
/*         (), */
/*       ), */
/*     ), */
/*     ~children, */
/*   ); */

/* let image = */
/*     ( */
/*       ~onMouseDown=?, */
/*       ~onMouseMove=?, */
/*       ~onMouseUp=?, */
/*       ~onMouseWheel=?, */
/*       ~children, */
/*       ~ref=?, */
/*       ~style=Style.defaultStyle, */
/*       ~src="", */
/*       (), */
/*     ) => */
/*   UiReact.primitiveComponent( */
/*     Image( */
/*       style, */
/*       src, */
/*       NodeEvents.make( */
/*         ~ref?, */
/*         ~onMouseDown?, */
/*         ~onMouseMove?, */
/*         ~onMouseUp?, */
/*         ~onMouseWheel?, */
/*         (), */
/*       ), */
/*     ), */
/*     ~children, */
/*   ); */

/* let text = */
/*     ( */
/*       ~onMouseDown=?, */
/*       ~onMouseMove=?, */
/*       ~onMouseUp=?, */
/*       ~onMouseWheel=?, */
/*       ~children: list(string), */
/*       ~ref=?, */
/*       ~style=Style.defaultStyle, */
/*       (), */
/*     ) => */
/*   UiReact.primitiveComponent( */
/*     Text( */
/*       style, */
/*       List.hd(children), */
/*       NodeEvents.make( */
/*         ~ref?, */
/*         ~onMouseDown?, */
/*         ~onMouseMove?, */
/*         ~onMouseUp?, */
/*         ~onMouseWheel?, */
/*         (), */
/*       ), */
/*     ), */
/*     ~children=[], */
/*   ); */
