open Revery_UI;

type clickFunction = unit => unit;
type clickFunctionWithEvt = NodeEvents.mouseButtonEventParams => unit;

/**
{2 Description:}

This module provides a `<Clickable />` component, loosely inspired by the
`<Touchable />` component in React-Native, but geared towards the desktop.

{2 Usage:}

{[
<Clickable onClick={(_) => print_endline("Clicked!")}>
  <Container width=100 height=100 />
</Clickable>
]}
 */
let createElement:
  (
    ~style: list(Revery_UI.Style.viewStyleProps)=?,
    ~onClick: clickFunction=?,
    ~onRightClick: clickFunction=?,
    ~onAnyClick: clickFunctionWithEvt=?,
    ~onBlur: Revery_UI.NodeEvents.focusHandler=?,
    ~onFocus: Revery_UI.NodeEvents.focusHandler=?,
    ~tabindex: int=?,
    ~children: list(Revery_UI.React.syntheticElement),
    ~onKeyDown: Revery_UI.NodeEvents.keyDownHandler=?,
    ~onKeyUp: Revery_UI.NodeEvents.keyUpHandler=?,
    ~onKeyPress: Revery_UI.NodeEvents.keyPressHandler=?,
    ~componentRef: Revery_UI.node => unit=?,
    unit
  ) =>
  Revery_UI.React.syntheticElement;
