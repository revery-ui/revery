open Revery_Core;
open Revery_UI;

/**
{2 Description:}

[ClipContainer] is similar to a container, except that it clips
any items outside.

{2 Usage:}

{[
<ClipContainer width=50 height=50>
  <Container width=100 height=100 color=Colors.green />
</ClipContainer
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
