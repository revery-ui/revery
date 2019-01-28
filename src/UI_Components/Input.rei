let createElement:
  (
    ~window: Revery_Core.Window.t,
    ~children: 'a,
    ~style: list(Revery_UI.Style.allProps)=?,
    ~placeholderColor: Revery_Core.Color.t=?,
    ~cursorColor: Revery_Core.Color.t=?,
    ~value: string=?,
    ~placeholder: string=?,
    ~onChange: (~value: string) => unit=?,
    unit
  ) =>
  Revery_UI.React.syntheticElement;
