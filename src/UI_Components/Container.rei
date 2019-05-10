open Revery_UI;

/**
Usage:

[<Container width=50 height=50 color=Colors.red />]

@param [width] The width of the container, in pixels.
@param [height] The height of the container, in pixels
*/
let createElement:
  (
    ~children: list(React.syntheticElement),
    ~width: int,
    ~height: int,
    unit
  ) =>
  React.syntheticElement;
