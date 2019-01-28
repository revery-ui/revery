type button = {
  text: string,
  value: string,
};

let createElement:
  (
    ~children: 'a,
    ~defaultSelected: int,
    ~buttons: list(button),
    ~iconSize: int=?,
    ~style: list(Revery_UI.Style.textStyleProps)=?,
    ~onChange: string => unit=?,
    unit
  ) =>
  Revery_UI.React.syntheticElement;
