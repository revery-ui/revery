let createElement:
  (
    ~children: 'a,
    ~onValueChanged: float => unit=?,
    ~minimumValue: float=?,
    ~maximumValue: float=?,
    ~value: float=?,
    ~vertical: bool=?,
    ~thumbLength: int=?,
    ~sliderLength: int=?,
    unit
  ) =>
  Revery_UI.React.syntheticElement;
