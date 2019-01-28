let createElement:
  (
    ~children: 'a,
    ~title: string,
    ~onClick: unit => unit=?,
    ~color: Revery_Core.Color.t=?,
    ~fontSize: int=?,
    ~width: int=?,
    ~height: int=?,
    ~onFocus: unit => unit=?,
    ~onBlur: unit => unit=?,
    ~tabindex: int=?,
    ~disabled: bool=?,
    ~fontFamily: string=?,
    unit
  ) =>
  Revery_UI.React.syntheticElement;
