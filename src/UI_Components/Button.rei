/**
{2  Description:}

Simple out-of-box button component

{2 Usage:}

{[
    <Button width=50 height=150 color=Colors.blue />
]}
*/
let make:
  (
    ~title: String.t,
    ~onClick: Revery_UI_Components__.Clickable.clickFunction=?,
    ~color: Revery_Core__.Color.t=?,
    ~fontSize: int=?,
    ~width: int=?,
    ~height: int=?,
    ~disabled: bool=?,
    ~tabindex: int=?,
    ~onFocus: Revery_UI.NodeEvents.focusHandler=?,
    ~onBlur: Revery_UI.NodeEvents.focusHandler=?,
    ~fontFamily: string=?,
    unit
  ) =>
  Revery_UI.React.element(Revery_UI.React.node);
