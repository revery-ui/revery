/**
{2 Description:}

Simple out-of-box checkbox component

{2 Usage:}

{[
    <Checkbox checked=true onChange={(_) => print_endline("Checkbox changed!")}/>
]}
*/

let createElement:
  (
    ~children: 'a,
    ~checked: bool=?,
    ~checkedColor: Color.Rgba'.t=?,
    ~style: list(Revery_UI.Style.viewStyleProps)=?,
    ~onChange: unit => unit=?,
    unit
  ) =>
  Revery_UI.React.syntheticElement;
