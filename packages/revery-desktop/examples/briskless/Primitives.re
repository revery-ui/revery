[@deriving show]
type size = [ | `Weight(int) | `Absolute(int)];

[@deriving show({with_path: false})]
type position = {
  x: int,
  y: int,
};

module FontManager = {
  let instance = Skia.FontManager.makeDefault();
};

module Font = {
  let skiaFaceToHarfbuzzFace = skiaFace => {
    let stream = Skia.Typeface.toStream(skiaFace);
    let length = Skia.Stream.getLength(stream);
    let data = Skia.Data.makeFromStream(stream, length);
    let bytes = Skia.Data.makeString(data);

    Harfbuzz.hb_face_from_data(bytes);
  };

  let loadHbface = typeface => {
    let harfbuzzFace = skiaFaceToHarfbuzzFace(typeface);

    switch (harfbuzzFace) {
    | Ok(hbFace) =>
      Console.log("Loaded: " ++ Skia.Typeface.getFamilyName(typeface));
      Ok(hbFace);
    | Error(msg) => Error("Error loading typeface: " ++ msg)
    };
  };

  let latest = ref(None);

  let resolve = (~italic, weight, family) => {
    let style = Skia.FontStyle.make(weight, 5, italic ? Italic : Upright);
    let maybeSkiaface =
      Skia.FontManager.matchFamilyStyle(FontManager.instance, family, style);

    switch (maybeSkiaface) {
    | Some(skiaface) =>
      loadHbface(skiaface) |> Result.map(hbface => (skiaface, hbface))
    | None => Error("Unable to find matching Skia typeface: " ++ family)
    };
  };
  let resolve = (~italic, weight, family) => {
    // TODO: proper cache, this is just a workaround to avoid crashing
    switch (latest^) {
    | Some(font) => Ok(font)
    | None =>
      resolve(~italic, weight, family)
      |> Result.map(font => {
           latest := Some(font);
           font;
         })
    };
  };

  let glyphsToString = shapes => {
    let len = Array.length(shapes);
    let bytes = Bytes.create(len * 2);

    let i = ref(0);

    while (i^ < len) {
      let idx = i^;
      let {glyphId, _}: Harfbuzz.hb_shape = shapes[idx];

      let lowBit = glyphId land 255;
      let highBit = (glyphId land 255 lsl 8) lsr 8;
      Bytes.set(bytes, idx * 2 + 0, Char.chr(lowBit));
      Bytes.set(bytes, idx * 2 + 1, Char.chr(highBit));

      incr(i);
    };

    Bytes.to_string(bytes);
  };

  let shape = (hbface, text) => {
    let glyphs = Harfbuzz.hb_shape(~features=[], hbface, text);

    glyphsToString(glyphs);
  };
};

module Constraint = {
  [@deriving show({with_path: false})]
  type t = {
    min: int,
    max: int,
  };

  // TODO maintain min < max invariant

  let tight = size => {min: size, max: size};

  let constrain = (size, self) => size |> min(self.max) |> max(self.min);

  let contractBy = (size, self) => {
    ...self,
    max: constrain(self.max - size, self),
  };
};

module BoxConstraints = {
  [@deriving show({with_path: false})]
  type t = {
    width: Constraint.t,
    height: Constraint.t,
  };

  let tight = (~width, ~height) => {
    width: Constraint.tight(width),
    height: Constraint.tight(height),
  };

  let contractBy = (~width, ~height, constraints) => {
    width: Constraint.contractBy(width, constraints.width),
    height: Constraint.contractBy(height, constraints.height),
  };
};

module Layout = {
  [@deriving show({with_path: false})]
  type t = {
    width: int,
    height: int,
  };

  let max = (constraints: BoxConstraints.t) => {
    width: constraints.width.max,
    height: constraints.height.max,
  };

  let min = (constraints: BoxConstraints.t) => {
    width: constraints.width.min,
    height: constraints.height.min,
  };
};

module BuildContext = {
  type t('element) = {mutable invalidated: list('element)};

  let invalidate = (element, buildContext) =>
    buildContext.invalidated = [element, ...buildContext.invalidated];
};

// module Element = {
//   type t('spec) = {V
//     spec: 'spec,
//     mutable state: option(Univ.t),
//     mutable layout: option(Layout.t),
//     mutable position: option(position),
//     mutable children: option(list(t('spec))),
//     mutable data,
//   }

//   and data =
//     | TextData({
//         skiaface: Skia.Typeface.t,
//         hbface: Harfbuzz.hb_face,
//         paint: Skia.Paint.t,
//         metrics: Skia.FontMetrics.t,
//         fontHeight: float,
//         lines: list(string),
//       })
//     | NoData;

//   let contains = (x, y, element) =>
//     switch (element) {
//     | {position: Some(position), layout: Some(layout), _} =>
//       x >= position.x
//       && y >= position.y
//       && x < position.x
//       + layout.width
//       && y < position.y
//       + layout.height
//     | _ => false
//     };
// };

//module Test = {
//  let hvoerState = State.create(false);
//
//  let hvoerState = initialValue => (~children as render, ()) =>
//    Widget.Effect({
//      mount: _invalidate => ref(initialValue),
//      render: (state, invalidate) => {
//        let setState = value => {
//          state.contents = value;
//          invalidate();
//        };
//
//        render(state.contents, setState);
//      },
//      unmount: _state => (),
//    });
//
//  let hvoerable = {
//    <hoverState>
//      ...{(isHovered, setHovered) => {
//        let color = isHovered ? Color.white : Color.gray;
//        <pointerTarget onMouseOver=setHovered(true) onMouseOut=setHovered(false) color />
//      }}
//    </hoverState>
//  };
//
//  let animated =
//    <tick every=Time.ms(10)/>
//      ...{dt => <rect x={x+dt} y />}
//    </tick>
//};

module TextElement = {
  [@deriving show({with_path: false})]
  type props = {
    text: string,
    fontFamily: string,
    size: float,
    weight: int,
    italic: bool,
    color: [@opaque] Skia.Color.t,
    lineHeight: float,
  };

  type t = {
    props,
    paint: Skia.Paint.t,
    skiaface: Skia.Typeface.t,
    hbface: Harfbuzz.hb_face,
    metrics: Skia.FontMetrics.t,
    fontHeight: float,
    mutable state,
  }

  and state =
    | New
    | LaidOut({lines: list(string)});

  let init =
      ({fontFamily, size, weight, italic, color, _} as props) =>
    switch (Font.resolve(~italic, weight, fontFamily)) {
    | Error(msg) =>
      Console.error(msg);
      failwith(msg) //TODO

    | Ok((skiaface, hbface)) =>
      let paint = Skia.Paint.make();
      Skia.Paint.setColor(paint, color);
      Skia.Paint.setTextEncoding(paint, GlyphId);
      Skia.Paint.setLcdRenderText(paint, true);
      Skia.Paint.setAntiAlias(paint, true);
      Skia.Paint.setTextSize(paint, size);
      Skia.Paint.setTypeface(paint, skiaface);

      let metrics = Skia.FontMetrics.make();
      let fontHeight = Skia.Paint.getFontMetrics(paint, metrics, 1.0);

      {props, paint, skiaface, hbface, metrics, fontHeight, state: New};
    };

  let layout = ({text, _}, {paint, hbface, fontHeight, _} as data, constraints: BoxConstraints.t) => {
    // TODO: Optimize shaping
    let lines =
      TextWrap.wrap(
        ~max_width=float(constraints.width.max),
        ~width_of_token=
          text => {
            let text = Font.shape(hbface, text);
            Skia.Paint.measureText(paint, text, None);
          },
        text,
      );
    let lines = List.map(Font.shape(hbface), lines);
    data.state = LaidOut({lines: lines});

    Layout.{
      width: constraints.width.max,
      height:
        int_of_float(fontHeight *. float(List.length(lines))),
    };
  };

  let render = ({props, paint, metrics, fontHeight, state, _}, canvas) => {
    let lines = 
      switch (state) {
      | LaidOut({lines}) => lines
      | New => []
      };

    List.iteri(
      (i, line) => {
        let ascentPx = Skia.FontMetrics.getAscent(metrics);
        let lineHeight = fontHeight *. props.lineHeight;

        // TODO: cache
        let baselineY = ascentPx *. (-1.0) +. lineHeight *. float(i);

        Skia.Canvas.drawText(canvas, line, 0., baselineY, paint);
      },
      lines,
    );
  };
};

module EffectElement = {
  type props = {
    equal: (Univ.t, Univ.t) => bool,
    mount: (~invalidate: unit => unit) => Univ.t,
    render: (~invalidate: unit => unit, Univ.t) => t,
    unmount: Univ.t => unit,
  };

  type t = {
    props,
    mutable state: Univ.t,
    mutable invalidated: bool
  };

  let init = ({mount, _} as props) => {
    let rec data = {
      state: mount(~invalidate=() => data.invalidated = true);
    };
    data
  }
};

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
  //  [@warning "-30"]
  type t =
    | Text(TextElement.props): t
    | ConstrainedBox({
        constraints: BoxConstraints.t,
        child: [@opaque] t,
      })
      : t
    | Box({
        color: [@opaque] Skia.Color.t,
        child: [@opaque] t,
      })
      : t
    | Clickable({
        onClick: unit => unit,
        child: [@opaque] t,
      })
      : t
    | Align(alignment, [@opaque] t): t
    | Padding({
        left: int,
        top: int,
        right: int,
        bottom: int,
        child: [@opaque] t,
      })
      : t
    | Container(layoutType, list((size, [@opaque] t))): t
    | Effect(Effect.props): t;
};


module WidgetElement = {
  type t = {
    spec: Widget.t,
    kind,
    mutable layout: option(Layout.t),
    mutable position: option(position),
    mutable children: option(list(t)),
  }

  and kind =
    | Text(TextElement.t)
    | Effect(Univ.t)
    | Other;

  let contains = (x, y, element) =>
    switch (element) {
    | {position: Some(position), layout: Some(layout), _} =>
      x >= position.x
      && y >= position.y
      && x < position.x
      + layout.width
      && y < position.y
      + layout.height
    | _ => false
    };

  let invalidate = (_element: t) => {
    () // TODO
  };

  let fromSpec = (spec: Widget.t) => {
    let kind =
      switch (spec) {
      | Text(props) =>
        Text(TextElement.init(props));

      | Effect({mount, _}) =>

      | _ => Other
      };

    let element =
      {
        spec,
        kind,
        layout: None,
        position: None,
        children: None,
      };

    element;
  };

  let retrieveChildren = (element: t) => {
    switch (element.spec) {
    | Text(_) => []

    | ConstrainedBox({child, _})
    | Box({child, _})
    | Clickable({child, _})
    | Align(_, child)
    | Padding({child, _}) => [fromSpec(child)]

    | Container(_, children) =>
      List.map(((_, spec)) => fromSpec(spec), children)

    | Effect({render, _}) =>
      switch (element.state) {
      | Some(state) =>
        let invalidate = () => invalidate(element);
        [render(state, ~invalidate) |> fromSpec];

      | None => failwith("Unreachable; WidgetElement.setChildren: no state")
      }
    };
  };

  let ensureChildren = (element: t) =>
    switch (element.children) {
    | Some(children) => children
    | None =>
      let children = retrieveChildren(element);
      element.children = Some(children);
      children;
    };

  let rec performLayout = (constraints: BoxConstraints.t, element: t) =>
    switch (element.layout) {
    | Some(_) => ()
    | None =>
      let childElements = ensureChildren(element);

      switch (element.kind) {
      | Text(data) =>
        // TODO: invalidate
        element.kind =
          Text.init(
            ~text,
            ~fontFamily,
            ~size,
            ~weight,
            ~italic,
            ~color,
            ~width=float(constraints.width.max),
          );
        element.layout = Some(Text.layout);

      | Container(/*Column, */ _) =>
        element.layout = Some(Layout.max(constraints));
        let itemWidth = constraints.width.max / List.length(childElements);
        let childConstraints =
          BoxConstraints.{
            ...constraints,
            width: Constraint.tight(itemWidth),
          };
        List.iteri(
          (i, child) => {
            performLayout(childConstraints, child);
            child.position = Some({x: i * itemWidth, y: 0});
          },
          childElements,
        );

      | ConstrainedBox({constraints: childConstraints, _}) =>
        element.layout = Some(Layout.max(childConstraints));
        List.iter(
          child => {
            performLayout(childConstraints, child);
            child.position = Some({x: 0, y: 0});
          },
          childElements,
        );

      | Box(_)
      | Clickable(_)
      | Effect(_) =>
        element.layout = Some(Layout.max(constraints));
        List.iter(
          child => {
            performLayout(constraints, child);
            child.position = Some({x: 0, y: 0});
          },
          childElements,
        );

      | Align(alignment, _) =>
        element.layout = Some(Layout.max(constraints));
        List.iter(
          child => {
            performLayout(constraints, child);
            let layout = Option.get(child.layout);
            let position =
              switch (alignment) {
              | `bottomCenter => {
                  x: (constraints.width.max - layout.width) / 2,
                  y: constraints.height.max - layout.height,
                }
              | `bottomLeft => {
                  x: 0,
                  y: constraints.height.max - layout.height,
                }
              | `bottomRight => {
                  x: constraints.width.max - layout.width,
                  y: constraints.height.max - layout.height,
                }
              | `center => {
                  x: (constraints.width.max - layout.width) / 2,
                  y: (constraints.height.max - layout.height) / 2,
                }
              | `centerLeft => {
                  x: 0,
                  y: constraints.height.max - layout.height,
                }
              | `centerRight => {
                  x: constraints.width.max - layout.width,
                  y: (constraints.height.max - layout.height) / 2,
                }
              | `topCenter => {
                  x: (constraints.width.max - layout.width) / 2,
                  y: 0,
                }
              | `topLeft => {x: 0, y: 0}
              | `topRight => {x: constraints.width.max - layout.width, y: 0}
              };
            child.position = Some(position);
          },
          childElements,
        );

      | Padding({left, top, right, bottom, _}) =>
        element.layout = Some(Layout.max(constraints));
        let childConstraints =
          BoxConstraints.contractBy(
            ~width=left - right,
            ~height=top - bottom,
            constraints,
          );
        List.iter(
          child => {
            performLayout(childConstraints, child);
            child.position = Some({x: left, y: top});
          },
          childElements,
        );
      };
    };

  let rec render = (~x=0, ~y=0, canvas, element: t) => {
    let position = Option.get(element.position);
    let layout = Option.get(element.layout);
    let children = Option.get(element.children);
    let x = x + position.x;
    let y = y + position.y;

    let transform = Skia.Matrix.makeTranslate(float(x), float(y));
    //    let transform = Skia.Matrix.makeTranslate(50., 50.);
    //    Console.log(layout.x);
    Skia.Canvas.setMatrix(canvas, transform);

    switch (element.spec) {
    | Text({lineHeight: lineHeightFactor, _}) =>
      Text.render();

    | Box({color, _}) =>
      let paint = Skia.Paint.make();
      Skia.Paint.setColor(paint, color);
      Skia.Canvas.drawRectLtwh(
        canvas,
        0.,
        0.,
        float(layout.width),
        float(layout.height),
        paint,
      );

    | Clickable(_)
    | ConstrainedBox(_)
    | Align(_)
    | Padding(_)
    | Container(_)
    | Effect(_) => ()
    };

    //    Console.log(List.length(layout.layout.children));
    List.iter(render(~x, ~y, canvas), children);
  };

  let handleClick = (~x, ~y, element) => {
    let rec loop = (~offsetX, ~offsetY, latest, elements) =>
      switch (elements) {
      | [] => latest
      | [element, ..._]
          when contains(x - offsetX, y - offsetY, element) =>
        let position = Option.get(element.position);
        let offsetX = offsetX + position.x;
        let offsetY = offsetY + position.y;
        switch (element.spec) {
        | Widget.Clickable({onClick, _}) =>
          loop(
            ~offsetX,
            ~offsetY,
            Some(onClick),
            element.children |> Option.get,
          )
        | _ =>
          loop(~offsetX, ~offsetY, latest, element.children |> Option.get)
        };
      | [_, ...rest] => loop(~offsetX, ~offsetY, latest, rest)
      };

    switch (loop(~offsetX=0, ~offsetY=0, None, [element])) {
    | Some(onClick) => onClick()
    | None => ()
    };
  };
};