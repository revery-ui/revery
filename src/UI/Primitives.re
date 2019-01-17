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
      ~onMouseWheel=?,
      ~onBlur=?,
      ~onFocus=?,
      ~tabindex=?,
      ~children,
      ~ref=?,
      ~style=Style.defaultStyle,
      (),
    ) =>
  UiReact.primitiveComponent(
    View(
      style,
      NodeEvents.make(
        ~ref?,
        ~onMouseDown?,
        ~onMouseMove?,
        ~onMouseUp?,
        ~onMouseWheel?,
        ~onFocus?,
        ~onBlur?,
        (),
      ),
      tabindex,
    ),
    ~children,
  );

let image =
    (
      ~onMouseDown=?,
      ~onMouseMove=?,
      ~onMouseUp=?,
      ~onMouseWheel=?,
      ~children,
      ~ref=?,
      ~style=Style.defaultStyle,
      ~src="",
      (),
    ) =>
  UiReact.primitiveComponent(
    Image(
      style,
      src,
      NodeEvents.make(
        ~ref?,
        ~onMouseDown?,
        ~onMouseMove?,
        ~onMouseUp?,
        ~onMouseWheel?,
        (),
      ),
    ),
    ~children,
  );

let text =
    (
      ~onMouseDown=?,
      ~onMouseMove=?,
      ~onMouseUp=?,
      ~onMouseWheel=?,
      ~children: list(string),
      ~ref=?,
      ~style=Style.defaultStyle,
      (),
    ) =>
  UiReact.primitiveComponent(
    Text(
      style,
      List.hd(children),
      NodeEvents.make(
        ~ref?,
        ~onMouseDown?,
        ~onMouseMove?,
        ~onMouseUp?,
        ~onMouseWheel?,
        (),
      ),
    ),
    ~children=[],
  );