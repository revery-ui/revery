/**
{2 Description:}

Simple out-of-box checkbox component

{2 Usage:}

{[
    <Checkbox checked=true onChange={(_) => print_endline("Checkbox changed!")}/>
]}
*/

let make:
  (
    ~checked: bool=?,
    ~checkedColor: Revery_Core__.Color.t=?,
    ~style: list(Revery_UI.Style.viewStyleProps)=?,
    ~onChange: Revery_UI_Components__.Clickable.clickFunction=?,
    unit
  ) =>
  Brisk_reconciler.element(Revery_UI.React.node);
