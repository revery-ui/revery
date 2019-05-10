open Revery_UI;

let createElement:
  (
    ~top: int=?,
    ~left: int=?,
    ~right: int=?,
    ~bottom: int=?,
    ~children: list(React.syntheticElement),
    unit
  ) =>
  React.syntheticElement;
