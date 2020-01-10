open Revery_UI;

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

let make:
  (
    ~key: Brisk_reconciler.Key.t=?,
    ~style: list(Revery_UI.Style.viewStyleProps)=?,
    ~onClick: unit => unit=?,
    ~onRightClick: unit => unit=?,
    ~onAnyClick: NodeEvents.mouseButtonEventParams => unit=?,
    ~componentRef: Revery_UI.node => unit=?,
    ~onBlur: Revery_UI.NodeEvents.focusHandler=?,
    ~onFocus: Revery_UI.NodeEvents.focusHandler=?,
    ~tabindex: int=?,
    ~onKeyDown: Revery_UI.NodeEvents.keyDownHandler=?,
    ~onKeyUp: Revery_UI.NodeEvents.keyUpHandler=?,
    ~onTextEdit: Revery_UI.NodeEvents.textEditHandler=?,
    ~onTextInput: Revery_UI.NodeEvents.textInputHandler=?,
    ~children: Brisk_reconciler.element(Revery_UI.React.node),
    unit
  ) =>
  Brisk_reconciler.element(Revery_UI.React.node);
