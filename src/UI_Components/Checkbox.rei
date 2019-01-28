let createElement:
  (
    ~children: 'a,
    ~checked: bool=?,
    ~checkedColor: Revery_Core.Color.t=?,
    ~style: list(Revery_UI.Style.viewStyleProps)=?,
    ~onChange: bool => unit=?,
    unit
  ) =>
  Revery_UI.React.syntheticElement;
