/*
 * Primitives
 *
 * These are the 'raw primitives' that our API describes.
 * These map directly to the raw nodes that get emitted in our scene graph.
 *
 * More details here:
 * https://github.com/bryphe/reason-reactify
 */

let view =
    (
      ~children,
      ~ref=NodeEvents.eventNoop,
      ~style=Style.defaultStyle,
      ~onMouseDown=NodeEvents.eventNoop,
      ~onMouseUp=NodeEvents.eventNoop,
      ~onMouseMove=NodeEvents.eventNoop,
      (),
    ) =>
  UiReact.primitiveComponent(
    View(
      style,
      NodeEvents.make(~ref, ~onMouseDown, ~onMouseMove, ~onMouseUp, ()),
    ),
    ~children,
  );

let image = (~children, ~style=Style.defaultStyle, ~src="", ()) =>
  UiReact.primitiveComponent(Image(style, src), ~children);

let text = (~children: list(string), ~style=Style.defaultStyle, ()) =>
  UiReact.primitiveComponent(Text(style, List.hd(children)), ~children=[]);
