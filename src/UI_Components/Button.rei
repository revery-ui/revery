/**
{2  Description:}

Simple out-of-box button component

{2 Usage:}

{[
    <Button width=50 height=150 color=Colors.blue title="Ok"/>
]}
*/
let make:
  (
    ~title: string,
    ~onClick: unit => unit=?,
    ~color: Revery_Core.Color.t=?,
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
  Brisk_reconciler.element(Revery_UI.React.node);
