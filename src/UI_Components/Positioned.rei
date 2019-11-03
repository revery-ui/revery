let make:
  (
    ~top: int=?,
    ~left: int=?,
    ~right: int=?,
    ~bottom: int=?,
    ~children: Revery_UI.React.React.element(Revery_UI.React.node),
    unit
  ) =>
  Brisk_reconciler.element(Revery_UI.viewNode);
