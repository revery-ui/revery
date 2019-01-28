let createElement:
  (
    ~style: list(Revery_UI.Style.viewStyleProps)=?,
    ~onClick: unit => unit=?,
    ~onBlur: Revery_UI.NodeEvents.focusHandler=?,
    ~onFocus: Revery_UI.NodeEvents.focusHandler=?,
    ~tabindex: int=?,
    ~children: list(Revery_UI.React.syntheticElement),
    unit
  ) =>
  Revery_UI.React.syntheticElement;
