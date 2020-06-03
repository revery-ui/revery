/**
 * Image.Asset
 *
 * Used for asset images.
 *
 * Examples:
 *   <Image.Asset src="example.png" opacity=1.0 width=64 height=64 />
 */
module Asset: {
  let make:
    (
      ~key: Brisk_reconciler.Key.t=?,
      ~onMouseDown: Revery_UI.NodeEvents.mouseDownHandler=?,
      ~onMouseMove: Revery_UI.NodeEvents.mouseMoveHandler=?,
      ~onMouseUp: Revery_UI.NodeEvents.mouseUpHandler=?,
      ~onMouseWheel: Revery_UI.NodeEvents.mouseWheelHandler=?,
      ~ref: Revery_UI.NodeEvents.refCallback(Revery_UI__.Node.node)=?,
      ~resizeMode: Revery_UI.ImageResizeMode.t=?,
      ~opacity: float=?,
      ~width: int=?,
      ~height: int=?,
      ~src: string=?,
      ~style: list(Revery_UI.Style.imageStyleProps)=?,
      ~children: Revery_UI.React.React.element(Revery_UI.React.node)=?,
      unit
    ) =>
    Brisk_reconciler.element(Revery_UI.React.node);
};

/**
 * Image.Network
 *
 * Used for images served over network, e.g. `https` etc.
 *
 * Examples:
 *   <Image.Network src="https://example.com/image.png" opacity=1.0 width=64 height=64 />
 */
module Network: {
  let make:
    (
      ~key: Brisk_reconciler.Key.t=?,
      ~onMouseDown: Revery_UI.NodeEvents.mouseDownHandler=?,
      ~onMouseMove: Revery_UI.NodeEvents.mouseMoveHandler=?,
      ~onMouseUp: Revery_UI.NodeEvents.mouseUpHandler=?,
      ~onMouseWheel: Revery_UI.NodeEvents.mouseWheelHandler=?,
      ~ref: Revery_UI.NodeEvents.refCallback(Revery_UI__.Node.node)=?,
      ~resizeMode: Revery_UI.ImageResizeMode.t=?,
      ~opacity: float=?,
      ~width: int=?,
      ~height: int=?,
      ~src: string=?,
      ~style: list(Revery_UI.Style.imageStyleProps)=?,
      ~children: Revery_UI.React.React.element(Revery_UI.React.node)=?,
      unit
    ) =>
    Brisk_reconciler.element(Revery_UI.React.node);
};
