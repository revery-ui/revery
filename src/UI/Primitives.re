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
      ~onMouseDown=?,
      ~onMouseMove=?,
      ~onMouseUp=?,
      ~children,
      ~style=Style.defaultStyle,
      (),
    ) =>
  UiReact.primitiveComponent(
    View(
      style,
      NodeEvents.make(~onMouseDown?, ~onMouseMove?, ~onMouseUp?, ()),
    ),
    ~children,
  );

let image =
    (
      ~onMouseDown=?,
      ~onMouseMove=?,
      ~onMouseUp=?,
      ~children,
      ~style=Style.defaultStyle,
      ~src="",
      (),
    ) =>
  UiReact.primitiveComponent(
    Image(
      style,
      src,
      NodeEvents.make(~onMouseDown?, ~onMouseMove?, ~onMouseUp?, ()),
    ),
    ~children,
  );

let text =
    (
      ~onMouseDown=?,
      ~onMouseMove=?,
      ~onMouseUp=?,
      ~children: list(string),
      ~style=Style.defaultStyle,
      (),
    ) =>
  UiReact.primitiveComponent(
    Text(
      style,
      List.hd(children),
      NodeEvents.make(~onMouseDown?, ~onMouseMove?, ~onMouseUp?, ()),
    ),
    ~children=[],
  );