open Revery_Core;
open Revery_UI;

/**
Description:

Basic component to act as a container for multiple child components.

Usage:

{[
    <Container width=50 height=50 color=Colors.red />
]}

@param [width] The width of the container, in pixels.
@param [height] The height of the container, in pixels.
@param [color] The color of the container.
*/
let createElement:
  (
    ~children: list(React.syntheticElement),
    ~color: Color.t=?,
    ~width: int,
    ~height: int,
    unit
  ) =>
  React.syntheticElement;
