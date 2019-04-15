open Revery_Core;

type renderFunction = React.syntheticElement => unit;

/**
  [start] is the entry point for creating a UI driven by the
  brisk-reconciler React infrastructure.

  This does the following:
  - Hook up a render loop
  - Take a React.syntheticElement and render it via GPU
  - Hook up GLFW events and convert them to Revery UI events

  [start] returns a function that can be used to update the UI.

  An example usage would be:
  [let update = start(window, <View />);]
  [update(<Button />);]
*/

let start: (Window.t, React.syntheticElement) => renderFunction;
