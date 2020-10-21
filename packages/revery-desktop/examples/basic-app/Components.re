open Primitives;

type itemData = {
  children: list(Widget.t),
  size: [ | `Weight(int) | `Absolute(int)],
};

let%nativeComponent rowItem = (~children, ~size, (), hooks) => (
  {
    make: () => {children: [], size},
    configureInstance: (~isFirstRender as _, data) => {...data, size},
    children,
    insertNode: (~parent, ~child, ~position as _) => {
      {...parent, children: [child, ...parent.children]};
    },
    deleteNode: (~parent, ~child as _, ~position) => {
      {
        ...parent,
        children:
          Base.List.filteri(~f=(i, _) => i != position, parent.children),
      };
    },
    moveNode: (~parent, ~child as _, ~from as _, ~to_ as _) => {
      parent;
    },
  },
  hooks,
);

let flex = (weight, child) =>
  <rowItem size={`Weight(weight)}> child </rowItem>;

let row = (~children, ()) =>
  <Widget
    layout={(~maxWidth, ~maxHeight, ~children) => {
      let childWidth = maxWidth / List.length(children);

      {
        width: maxWidth,
        height: maxHeight,
        children:
          List.mapi(
            (i, child) =>
              Widget.{
                x: {
                  //            Console.log("layout");
                  //      Console.log(i * childWidth);
                  i * childWidth;
                },
                y: 0,
                layout:
                  child.layout(
                    ~maxWidth=childWidth,
                    ~maxHeight,
                    ~children=child.children,
                  ),
                widget: child,
              },
            children,
          ),
      };
    }}>
    children
  </Widget>;

let box = (~label, ~color, ()) =>
  <Widget
    layout={(~maxWidth, ~maxHeight, ~children) =>
      {
        width: maxWidth,
        height: maxHeight,
        children:
          List.map(
            child =>
              Widget.{
                x: 0,
                y: 0,
                layout:
                  child.layout(
                    ~maxWidth,
                    ~maxHeight,
                    ~children=child.children,
                  ),
                widget: child,
              },
            children,
          ),
      }
    }
    render={(~width, ~height, canvas) => {
      let paint = Skia.Paint.make();
      Skia.Paint.setColor(paint, color);
      Skia.Canvas.drawRectLtwh(
        canvas,
        0.,
        0.,
        float(width),
        float(height),
        paint,
      );

      // text
      Skia.Paint.setColor(
        paint,
        Skia.Color.makeArgb(0xFFl, 0xFFl, 0xFFl, 0xFFl),
      );

      Console.log("render " ++ label);

      Skia.Canvas.drawText(canvas, "Hello, world!!!", 50., 50., paint);
    }}
  />;

let text = (~text, ~color, ()) =>
  <Widget
    layout={(~maxWidth, ~maxHeight, ~children) =>
      {
        width: maxWidth,
        height: maxHeight,
        children:
          List.map(
            child =>
              Widget.{
                x: 0,
                y: 0,
                layout:
                  child.layout(
                    ~maxWidth,
                    ~maxHeight,
                    ~children=child.children,
                  ),
                widget: child,
              },
            children,
          ),
      }
    }
    render={(~width as _, ~height as _, canvas) => {
      let paint = Skia.Paint.make();
      Skia.Paint.setColor(paint, color);

      Skia.Canvas.drawText(canvas, text, 50., 50., paint);
    }}
  />;
