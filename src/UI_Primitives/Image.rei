/**
 * Image
 *
 * Used for rendering images. The `src`-property takes a `File(string) or a `Url(string)
 *
 * Examples:
 *   <Image src=`File("example.png") opacity=1.0 width=64 height=64 />
 *   <Image src=`Url("https://example.com/image.png") opacity=1.0 width=64 height=64 />
 */
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
    ~quality: [ | `none | `low | `medium | `high]=?,
    ~src: [ | `File(string) ],
    ~style: list(Revery_UI.Style.imageStyleProps)=?,
    ~children: Revery_UI.React.React.element(Revery_UI.React.node)=?,
    unit
  ) =>
  Brisk_reconciler.element(Revery_UI.React.node);
