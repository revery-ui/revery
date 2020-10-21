[@deriving show]
type size = [ | `Weight(int) | `Absolute(int)];

// WIDGET

module Widget = {
  [@deriving show({with_path: false})]
  type alignment = [
    | `bottomCenter
    | `bottomLeft
    | `bottomRight
    | `center
    | `centerLeft
    | `centerRight
    | `topCenter
    | `topLeft
    | `topRight
  ];

  [@deriving show({with_path: false})]
  type layoutType =
    | Row
    | Column;

  [@deriving show({with_path: false})]
  type constraints = {
    minWidth: int,
    maxWidth: int,
    minHeight: int,
    maxHeight: int,
  };

  [@deriving show({with_path: false})]
  //  [@warning "-30"]
  type t =
    | Text(string)
    | ConstrainedBox({
        constraints,
        child: [@opaque] t,
      })
    | Box({
        color: [@opaque] Skia.Color.t,
        child: [@opaque] t,
      })
    | Clickable({
        onClick: unit => unit,
        child: [@opaque] t,
      })
    | Align(alignment, t)
    | Padding({left: int, top: int, right: int, bottom: int, child: t})
    | Container(layoutType, list((size, [@opaque] t)));

  [@deriving show({with_path: false})]
  type layout = {
    width: int,
    height: int,
    children: list(positionedLayout),
  }

  [@deriving show({with_path: false})]
  and positionedLayout = {
    x: int,
    y: int,
    widget: t,
    layout,
  };

  let rec layout = (constraints, widget) => {
    let {minWidth, maxWidth, minHeight, maxHeight} = constraints;

    switch (widget) {
    | Container(Row, items) =>
      let itemWidth = maxWidth / List.length(items);

      {
        width: maxWidth,
        height: maxHeight,
        children:
          List.mapi(
            (i, (_, item)) =>
              {
                x: i * itemWidth,
                y: 0,
                layout:
                  layout(
                    {
                      minWidth: itemWidth,
                      maxWidth: itemWidth,
                      minHeight,
                      maxHeight,
                    },
                    item,
                  ),
                widget: item,
              },
            items,
          ),
      };

    | Clickable({child, _})
    | Box({child, _}) =>
      let layout = layout(constraints, child);
      {
        width: maxWidth,
        height: maxHeight,
        children: [{x: 0, y: 0, widget: child, layout}],
      };

    | Align(`bottomCenter, child) =>
      let layout = layout(constraints, child);
      {
        width: maxWidth,
        height: maxHeight,
        children: [
          {
            x: (maxWidth - layout.width) / 2,
            y: maxHeight - layout.height,
            widget: child,
            layout,
          },
        ],
      };

    | Align(`bottomLeft, child) =>
      let layout = layout(constraints, child);
      {
        width: maxWidth,
        height: maxHeight,
        children: [
          {x: 0, y: maxHeight - layout.height, widget: child, layout},
        ],
      };

    | Align(`bottomRight, child) =>
      let layout = layout(constraints, child);
      {
        width: maxWidth,
        height: maxHeight,
        children: [
          {
            x: maxWidth - layout.width,
            y: maxHeight - layout.height,
            widget: child,
            layout,
          },
        ],
      };

    | Align(`center, child) =>
      let layout = layout(constraints, child);
      {
        width: maxWidth,
        height: maxHeight,
        children: [
          {
            x: (maxWidth - layout.width) / 2,
            y: (maxHeight - layout.height) / 2,
            widget: child,
            layout,
          },
        ],
      };

    | Align(`centerLeft, child) =>
      let layout = layout(constraints, child);
      {
        width: maxWidth,
        height: maxHeight,
        children: [
          {x: 0, y: (maxHeight - layout.height) / 2, widget: child, layout},
        ],
      };

    | Align(`centerRight, child) =>
      let layout = layout(constraints, child);
      {
        width: maxWidth,
        height: maxHeight,
        children: [
          {
            x: maxWidth - layout.width,
            y: (maxHeight - layout.height) / 2,
            widget: child,
            layout,
          },
        ],
      };

    | Align(`topCenter, child) =>
      let layout = layout(constraints, child);
      {
        width: maxWidth,
        height: maxHeight,
        children: [
          {x: (maxWidth - layout.width) / 2, y: 0, widget: child, layout},
        ],
      };

    | Align(`topLeft, child) =>
      let layout = layout(constraints, child);
      {
        width: maxWidth,
        height: maxHeight,
        children: [{x: 0, y: 0, widget: child, layout}],
      };

    | Align(`topRight, child) =>
      let layout = layout(constraints, child);
      {
        width: maxWidth,
        height: maxHeight,
        children: [
          {x: maxWidth - layout.width, y: 0, widget: child, layout},
        ],
      };

    | Padding({left, top, right, bottom, child}) =>
      let constraints = {
        maxWidth: maxWidth - left - right,
        maxHeight: maxHeight - top - bottom,
        minWidth: max(minWidth, maxWidth - left - right),
        minHeight: max(minHeight, maxHeight - top - bottom),
      };

      {
        width: maxWidth,
        height: maxHeight,
        children: [
          {
            x: left,
            y: top,
            widget: child,
            layout: layout(constraints, child)
          },
        ],
      };

    | ConstrainedBox({constraints, child}) =>
      let layout = layout(constraints, child);
      {...layout, children: [{x: 0, y: 0, widget: child, layout}]};

    | Text(_)
    | Container(Column, _) => {
        width: maxWidth,
        height: maxHeight,
        children: [],
      }
    };
  };

  let rec globalizeLayoutPositions = (~x=0, ~y=0, layout) => {
    let x = x + layout.x;
    let y = y + layout.y;

    {
      ...layout,
      x,
      y,
      layout: {
        ...layout.layout,
        children: List.map(globalizeLayoutPositions(~x, ~y), layout.layout.children)
      }
    }
  };

  let rec render = (canvas, layout: positionedLayout) => {
//    let x = x + layout.x;
//    let y = y + layout.y;
    let transform = Skia.Matrix.makeTranslate(float(layout.x), float(layout.y));
    //    let transform = Skia.Matrix.makeTranslate(50., 50.);
    //    Console.log(layout.x);
    Skia.Canvas.setMatrix(canvas, transform);

    switch (layout.widget) {
    | Text(text) =>
      let paint = Skia.Paint.make();
      Skia.Paint.setColor(
        paint,
        Skia.Color.makeArgb(0xFFl, 0xFFl, 0xFFl, 0xFFl),
      );
      Skia.Canvas.drawText(canvas, text, 0., 50., paint);

    | Box({color, _}) =>
      let paint = Skia.Paint.make();
      Skia.Paint.setColor(paint, color);
      Skia.Canvas.drawRectLtwh(
        canvas,
        0.,
        0.,
        float(layout.layout.width),
        float(layout.layout.height),
        paint,
      );

    | Clickable(_)
    | ConstrainedBox(_)
    | Align(_)
    | Padding(_)
    | Container(_) => ()
    };

    //    Console.log(List.length(layout.layout.children));
    List.iter(render(canvas), layout.layout.children);
  };

  let contains = (x, y, layout) =>
    x >= layout.x
    && y >= layout.y
    && x < layout.x
    + layout.layout.width
    && y < layout.y
    + layout.layout.height;

  let handleClick = (~x, ~y, layout) => {
    let rec loop = (latest, layouts) =>
      switch (layouts) {
      | [] => latest
      | [layout, ..._] when contains(x, y, layout) =>
        switch (layout.widget) {
        | Clickable({onClick, _}) =>
          loop(Some(onClick), layout.layout.children)
        | _ => loop(latest, layout.layout.children)
        }
      | [_, ...rest] => loop(latest, rest)
      };

    switch (loop(None, [layout])) {
    | Some(onClick) => onClick()
    | None => ()
    };
  };
};
