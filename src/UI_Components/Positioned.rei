open Revery_UI;

let make:
  (
    ~top: int=?,
    ~left: int=?,
    ~right: int=?,
    ~bottom: int=?,
    ~children: list(React.syntheticElement),
    unit
  ) =>
  React.syntheticElement;
