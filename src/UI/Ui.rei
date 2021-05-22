open Revery_Core;

type renderFunction = React.element(React.reveryNode) => unit;

/**
  [start] is the entry point for creating a UI driven by the
  brisk-reconciler React infrastructure.

  This does the following:
  - Hook up a render loop
  - Take a React.element and render it via GPU
  - Hook up GLFW events and convert them to Revery UI events

  [start] returns a function that can be used to update the UI.

  An example usage would be:
  [let update = start(window, <View />);]
  [update(<Button />);]
*/

let start:
  (
    ~onBeforeRender: unit => unit=?,
    ~onAfterRender: unit => unit=?,
    Window.t,
    React.element(React.reveryNode)
  ) =>
  renderFunction;

let getActiveWindow: unit => option(Window.t);

/** [mouseBehavior] is an alias for SDL2's hit-test results.
       Each option is fairly self explanatory, but please note that
       these only work if you set `decorated` to false on window
       creation. This is unfortunately due to the fact that enabling
       hit tests causes the default chrome behavior on Windows to no
       longer work.
   */
type mouseBehavior =
  Sdl2.Window.hitTestResult =
    | Normal
    | Draggable
    | ResizeTopLeft
    | ResizeTop
    | ResizeTopRight
    | ResizeRight
    | ResizeBottomRight
    | ResizeBottom
    | ResizeBottomLeft
    | ResizeLeft;
