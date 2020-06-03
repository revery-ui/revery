// TDDO: 4.10 polyfill. Remove when safe to do so
module List = {
  include Stdlib.List;

  let rec find_map = f =>
    fun
    | [] => None
    | [head, ...tail] =>
      switch (f(head)) {
      | Some(_) as result => result
      | None => find_map(f, tail)
      };
};

// MODEL

type point = {
  x: float,
  y: float,
};

type size = {
  width: float,
  height: float,
};

type length = [
  | `zero
  | `user(float)
  | `em(float)
  | `ex(float)
  | `px(float)
  | `in_(float)
  | `cm(float)
  | `mm(float)
  | `pt(float)
  | `pc(float)
];
type percentage = [ | `percentage(float)];

type simplePaint = [ | `none | `currentColor | `color(Skia.Color.t)];
type paint = [ simplePaint | `inherit_ | `funciri(string, simplePaint)];

module Style = {};

module Attribute = {
  type presentation = [
    | `fill(paint)
    | `stroke(paint)
    | `strokeWidth(length)
  ];

  type t = [ presentation];
};

module Geometry = {
  type pathCommand = [
    // MoveTo
    | `M(float, float) // x, y
    | `m(float, float) // dx, dy
    // LineTo
    | `L(float, float) // x, y
    | `l(float, float) // dx, dy
    | `H(float) // x
    | `h(float) // dx
    | `V(float) // y
    | `v(float) // dy
    // CubicBezierCurve
    | `C(float, float, float, float, float, float) // x1, y1, x2, y2, x, y
    | `c(float, float, float, float, float, float) // dx1, dy1, dx2, dy2, dx, dy
    | `S(float, float, float, float) // x2, y2, x, y
    | `s(float, float, float, float) // dx2, dy2, dx, dy
    // QuadraticBezierCurve
    | `Q(float, float, float, float) // x1, y1, x, y
    | `q(float, float, float, float) // dx1, dy1, dx, dy
    | `T(float, float) // x, y
    | `t(float, float) // dx, dy
    // EllipticalArcCurve
    | `A(
        float,
        float,
        float,
        [ | `large | `small],
        [ | `cw | `ccw],
        float,
        float,
      ) // rx, ry, angle, large-arc-flag, sweep-flag, x, y
    | `a(
        float,
        float,
        float,
        [ | `large | `small],
        [ | `cw | `ccw],
        float,
        float,
      ) // rx, ry, angle, large-arc-flag, sweep-flag, dx, dy
    // ClosePath
    | `Z
    | `z
  ];

  type kind =
    | Circle({
        cx: [ length | percentage],
        cy: [ length | percentage],
        r: length,
      })
    | Ellipse({
        cx: [ length | percentage],
        cy: [ length | percentage],
        rx: [ | `auto | length | percentage],
        ry: [ | `auto | length | percentage],
      })
    | Line({
        x1: [ length | percentage],
        y1: [ length | percentage],
        x2: [ length | percentage],
        y2: [ length | percentage],
      })
    | Path({d: list(pathCommand)})
    | Polygon({points: list(point)})
    | Rect({
        x: [ length | percentage],
        y: [ length | percentage],
        width: [ | `auto | length | percentage],
        height: [ | `auto | length | percentage],
        rx: [ | `auto | length | percentage],
        ry: [ | `auto | length | percentage],
      });

  type t = {
    attributes: list(Attribute.t),
    kind,
  };
};

type element =
  | Group(list(element))
  | Geometry(Geometry.t)
  | Text(string);

type definition = {id: string};

type viewport = {
  origin: point,
  width: float,
  height: float,
};

type t = {
  defs: list(definition),
  elements: list(element),
  viewport,
};

// PARSER

module Parser = {
  module SimpleXml = {
    [@deriving show({with_path: false})]
    type t =
      | Element(string, list((string, string)), list(t))
      | Text(string);

    let trim =
      Markup.filter(
        fun
        | `Text(strs) when strs |> String.concat("") |> String.trim == "" =>
          false
        | _ => true,
      );

    let simplify = stream =>
      stream
      |> trim
      |> Markup.tree(
           ~text=strings => Text(strings |> String.concat("")),
           ~element=
             ((_ns, name), attrs, children) => {
               let attrs =
                 attrs |> List.map((((_ns, name), value)) => (name, value));
               Element(name, attrs, children);
             },
         );

    let of_file = path =>
      Markup.file(path)
      |> fst
      |> Markup.parse_xml
      |> Markup.signals
      |> simplify;

    let of_string = str =>
      Markup.string(str) |> Markup.parse_xml |> Markup.signals |> simplify;
  };

  open SimpleXml;

  let length = str => `user(float_of_string(str)); // TODO
  let length_percentage = str => `user(float_of_string(str)); // TODO
  let length_percentage_auto = str => `user(float_of_string(str)); // TODO

  let paint =
    fun
    | "none" => `none
    | "currentColor" => `currentColor
    | value => `color(Revery_Core.Color.(hex(value) |> toSkia)); // TODO

  let attribute =
    fun
    | ("fill", value) => Some(`fill(paint(value)))
    | ("stroke", value) => Some(`stroke(paint(value)))
    | ("stroke-width", value) => Some(`strokeWidth(length(value)))
    | _ => None;

  let pathCommands = str => {
    let tokens = {
      let buffer = Buffer.create(10);
      let acc = ref([]);

      let pushArg = () =>
        if (Buffer.length(buffer) > 0) {
          let token = Buffer.contents(buffer) |> float_of_string;
          acc := [`arg(token), ...acc^];
          Buffer.clear(buffer);
        };

      String.iter(
        fun
        // argument
        | ('0'..'9' | '.' | '+' | '-') as ch => Buffer.add_char(buffer, ch)
        // white-space
        | '\t'
        | ' '
        | '\n'
        | '\012'
        | '\r' => pushArg()
        | ',' => pushArg()
        // commands
        | (
            'M' | 'm' | 'L' | 'l' | 'H' | 'h' | 'V' | 'v' | 'C' | 'c' | 'S' |
            's' |
            'Q' |
            'q' |
            'T' |
            't' |
            'A' |
            'a' |
            'Z' |
            'z'
          ) as ch => {
            pushArg();
            acc := [`cmd(ch), ...acc^];
          }
        | ch =>
          failwith("invalid character in path data: " ++ Char.escaped(ch)),
        str,
      );

      pushArg();

      List.rev(acc^);
    };

    let rec parse = acc =>
      fun
      | [] => List.rev(acc)
      | [`cmd('M'), `arg(x), `arg(y), ...rest] =>
        parse([`M((x, y)), ...acc], rest)
      | [`cmd('m'), `arg(dx), `arg(dy), ...rest] =>
        parse([`m((dx, dy)), ...acc], rest)
      | [`cmd('L'), `arg(x), `arg(y), ...rest] =>
        parse([`L((x, y)), ...acc], rest)
      | [`cmd('l'), `arg(dx), `arg(dy), ...rest] =>
        parse([`l((dx, dy)), ...acc], rest)
      | [`cmd('H'), `arg(x), ...rest] => parse([`H(x), ...acc], rest)
      | [`cmd('h'), `arg(dx), ...rest] => parse([`h(dx), ...acc], rest)
      | [`cmd('V'), `arg(y), ...rest] => parse([`V(y), ...acc], rest)
      | [`cmd('v'), `arg(dy), ...rest] => parse([`V(dy), ...acc], rest)
      | [
          `cmd('C'),
          `arg(x1),
          `arg(y1),
          `arg(x2),
          `arg(y2),
          `arg(x),
          `arg(y),
          ...rest,
        ] =>
        parse([`C((x1, y1, x2, y2, x, y)), ...acc], rest)
      | [
          `cmd('c'),
          `arg(dx1),
          `arg(dy1),
          `arg(dx2),
          `arg(dy2),
          `arg(dx),
          `arg(dy),
          ...rest,
        ] =>
        parse([`c((dx1, dy1, dx2, dy2, dx, dy)), ...acc], rest)
      | [`cmd('S'), `arg(x2), `arg(y2), `arg(x), `arg(y), ...rest] =>
        parse([`S((x2, y2, x, y)), ...acc], rest)
      | [`cmd('s'), `arg(dx2), `arg(dy2), `arg(dx), `arg(dy), ...rest] =>
        parse([`s((dx2, dy2, dx, dy)), ...acc], rest)
      | [`cmd('Q'), `arg(x1), `arg(y1), `arg(x), `arg(y), ...rest] =>
        parse([`Q((x1, y1, x, y)), ...acc], rest)
      | [`cmd('q'), `arg(dx1), `arg(dy1), `arg(dx), `arg(dy), ...rest] =>
        parse([`q((dx1, dy1, dx, dy)), ...acc], rest)
      | [`cmd('T'), `arg(x), `arg(y), ...rest] =>
        parse([`T((x, y)), ...acc], rest)
      | [`cmd('t'), `arg(dx), `arg(dy), ...rest] =>
        parse([`t((dx, dy)), ...acc], rest)
      | [
          `cmd('A'),
          `arg(rx),
          `arg(ry),
          `arg(angle),
          `arg(largeArc),
          `arg(sweep),
          `arg(x),
          `arg(y),
          ...rest,
        ] =>
        parse(
          [
            `A((
              rx,
              ry,
              angle,
              largeArc == 0. ? `large : `small,
              sweep == 0. ? `ccw : `cw,
              x,
              y,
            )),
            ...acc,
          ],
          rest,
        )
      | [
          `cmd('a'),
          `arg(rx),
          `arg(ry),
          `arg(angle),
          `arg(largeArc),
          `arg(sweep),
          `arg(dx),
          `arg(dy),
          ...rest,
        ] =>
        parse(
          [
            `A((
              rx,
              ry,
              angle,
              largeArc == 0. ? `large : `small,
              sweep == 0. ? `ccw : `cw,
              dx,
              dy,
            )),
            ...acc,
          ],
          rest,
        )
      | [`cmd('Z'), ...rest] => parse([`Z, ...acc], rest)
      | [`cmd('z'), ...rest] => parse([`z, ...acc], rest)
      | [`arg(_arg), ..._rest] => failwith("TODO")
      | [`cmd(ch), ...rest] => {
          Console.log(rest);
          failwith("invalid command: " ++ Char.escaped(ch));
        };

    parse([], tokens);
  };

  let attr_length = (key, ~default, attrs) =>
    List.assoc_opt(key, attrs)
    |> Option.map(length)
    |> Option.value(~default);

  let attr_length_percentage = (key, ~default, attrs) =>
    List.assoc_opt(key, attrs)
    |> Option.map(length_percentage)
    |> Option.value(~default);

  let attr_length_percentage_auto = (key, ~default, attrs) =>
    List.assoc_opt(key, attrs)
    |> Option.map(length_percentage_auto)
    |> Option.value(~default);

  let attr_pathCommands = (key, ~default, attrs) =>
    List.assoc_opt(key, attrs)
    |> Option.map(pathCommands)
    |> Option.value(~default);

  let geometry = (kind, attrs) =>
    Geometry.{kind, attributes: List.filter_map(attribute, attrs)};

  let circle = attrs =>
    geometry(
      Circle({
        cx: attr_length_percentage("cx", ~default=`zero, attrs),
        cy: attr_length_percentage("cy", ~default=`zero, attrs),
        r: attr_length("r", ~default=`zero, attrs),
      }),
      attrs,
    );

  let ellipse = attrs =>
    geometry(
      Ellipse({
        cx: attr_length_percentage("cx", ~default=`zero, attrs),
        cy: attr_length_percentage("cy", ~default=`zero, attrs),
        rx: attr_length("rx", ~default=`auto, attrs),
        ry: attr_length("ry", ~default=`auto, attrs),
      }),
      attrs,
    );

  let line = attrs =>
    geometry(
      Line({
        x1: attr_length_percentage("x1", ~default=`zero, attrs),
        y1: attr_length_percentage("y1", ~default=`zero, attrs),
        x2: attr_length_percentage("x2", ~default=`zero, attrs),
        y2: attr_length_percentage("y2", ~default=`zero, attrs),
      }),
      attrs,
    );

  let path = attrs =>
    geometry(Path({d: attr_pathCommands("d", ~default=[], attrs)}), attrs);

  let rect = attrs =>
    geometry(
      Rect({
        x: attr_length_percentage("x", ~default=`zero, attrs),
        y: attr_length_percentage("y", ~default=`zero, attrs),
        width: attr_length_percentage_auto("width", ~default=`auto, attrs),
        height: attr_length_percentage_auto("height", ~default=`auto, attrs),
        rx: attr_length_percentage_auto("rx", ~default=`auto, attrs),
        ry: attr_length_percentage_auto("ry", ~default=`auto, attrs),
      }),
      attrs,
    );

  let rec element =
    fun
    | Element("defs", _, _) => failwith("TODO")
    | Element("g", _, children) =>
      Some(Group(List.filter_map(element, children)))
    | Element("circle", attrs, _children) => Some(Geometry(circle(attrs)))
    | Element("ellipse", attrs, _children) =>
      Some(Geometry(ellipse(attrs)))
    | Element("line", attrs, _children) => Some(Geometry(line(attrs)))
    | Element("path", attrs, _children) => Some(Geometry(path(attrs)))
    | Element("rect", attrs, _children) => Some(Geometry(rect(attrs)))
    | _ => None;

  let svg =
    fun
    | Element("svg", _, children) => {
        defs: [],
        elements: List.filter_map(element, children),
        viewport: {
          origin: {
            x: 0.,
            y: 0.,
          },
          width: 100.,
          height: 100.,
        },
      }
    | _ => failwith("svg root element expected");
};

// RENDERER

module SkiaRenderer = {
  open Revery_Draw;

  type font = {
    family: string,
    size: float,
    xHeight: float, // '1 ex', x-height of the elementâs font
    chWidth: float // '1 ch', width of the "0" (ZERO, U+0030) glyph in the elementâs font
  };

  type context = {
    defs: list(definition),
    font,
    viewport,
    container: size,
    rootFontSize: float, // '1 rem', font size of the root element
    canvas: CanvasContext.t,
  };

  let userCoord =
    fun
    | `zero => 0.
    | `user(n) => n
    | `em(_)
    | `ex(_)
    | `px(_)
    | `in_(_)
    | `cm(_)
    | `mm(_)
    | `pt(_)
    | `pc(_) => failwith("TODO")
    | `percentage(_) => failwith("TODO")
    | `auto => failwith("TODO");

  let paint =
    fun
    | `none => Skia.Paint.make()
    | `currentColor => failwith("TODO")
    | `color(color) => {
        let paint = Skia.Paint.make();
        Skia.Paint.setColor(paint, color);
        paint;
      }
    | `funciri(_) => failwith("TODO")
    | `inherit_ => failwith("TODO");

  let circle = (~cx, ~cy, ~r, ~paint, ~context) => {
    CanvasContext.drawCircle(~x=cx, ~y=cy, ~radius=r, ~paint, context.canvas);
  };

  let ellipse = (~cx, ~cy, ~rx, ~ry, ~paint, ~context) => {
    CanvasContext.drawOval(
      ~rect=Skia.Rect.makeLtrb(cx -. rx, cy -. ry, cx +. rx, cy +. ry),
      ~paint,
      context.canvas,
    );
  };

  let line = (~x1, ~y1, ~x2, ~y2, ~paint, ~context) => {
    let path = Skia.Path.make();
    Skia.Path.moveTo(path, x1, y1);
    Skia.Path.lineTo(path, x2, y2);
    CanvasContext.drawPath(~path, ~paint, context.canvas);
  };

  let path = (~d, ~paint, ~context) => {
    let path = Skia.Path.make();
    List.iter(
      fun
      | `M(x, y) => Skia.Path.moveTo(path, x, y)
      | `m(dx, dy) => Skia.Path.rMoveTo(path, dx, dy)
      | `L(x, y) => Skia.Path.lineTo(path, x, y)
      | `l(dx, dy) => Skia.Path.rLineTo(path, dx, dy)
      | `H(_) => failwith("TODO")
      | `h(dx) => Skia.Path.rLineTo(path, dx, 0.)
      | `V(_) => failwith("TODO")
      | `v(dy) => Skia.Path.rLineTo(path, 0., dy)
      | `C(x1, y1, x2, y2, x, y) =>
        Skia.Path.cubicTo(path, x1, y1, x2, y2, x, y)
      | `c(dx1, dy1, dx2, dy2, dx, dy) =>
        Skia.Path.rCubicTo(path, dx1, dy1, dx2, dy2, dx, dy)
      | `S(_) => failwith("TODO")
      | `s(_) => failwith("TODO")
      | `Q(_) => failwith("TODO")
      | `q(_) => failwith("TODO")
      | `T(_) => failwith("TODO")
      | `t(_) => failwith("TODO")
      | `A(_) => failwith("TODO")
      | `a(_) => failwith("TODO")
      | `Z
      | `z => Skia.Path.close(path),
      d,
    );
    CanvasContext.drawPath(~path, ~paint, context.canvas);
  };

  let rect = (~x, ~y, ~width, ~height, ~rx, ~ry, ~paint, ~context) => {
    CanvasContext.drawRoundRect(
      ~rect=Skia.Rect.makeLtrb(x, y, x +. width, y +. height),
      ~rx,
      ~ry,
      ~paint,
      context.canvas,
    );
  };

  let geometry = (context, shape: Geometry.t) => {
    let scaleX = context.container.width /. context.viewport.width;
    let scaleY = context.container.height /. context.viewport.height;
    let translateX = context.viewport.origin.x *. scaleX;
    let translateY = context.viewport.origin.y *. scaleY;

    let transform = Skia.Matrix.make();
    Skia.Matrix.setAll(
      transform,
      scaleX,
      0.,
      translateX,
      0.,
      scaleY,
      translateY,
      0.,
      0.,
      1.,
    );
    CanvasContext.setMatrix(context.canvas, transform);

    let fill =
      List.find_map(
        fun
        | `fill(spec) => Some(paint(spec))
        | _ => None,
        shape.attributes,
      );

    let stroke = {
      let make = spec => {
        let paint = paint(spec);

        Skia.Paint.setStyle(paint, Stroke);

        List.iter(
          fun
          | `strokeWidth(width) =>
            Skia.Paint.setStrokeWidth(paint, userCoord(width))
          | _ => (),
          shape.attributes,
        );

        Some(paint);
      };

      List.find_map(
        fun
        | `stroke(spec) => make(spec)
        | _ => None,
        shape.attributes,
      );
    };

    switch (shape.kind) {
    | Circle({cx, cy, r}) =>
      let cx = userCoord(cx);
      let cy = userCoord(cy);
      let r = userCoord(r);
      let draw = paint => circle(~cx, ~cy, ~r, ~paint, ~context);
      Option.iter(draw, fill);
      Option.iter(draw, stroke);

    | Ellipse({cx, cy, rx, ry}) =>
      let cx = userCoord(cx);
      let cy = userCoord(cy);
      let rx = userCoord(rx);
      let ry = userCoord(ry);
      let draw = paint => ellipse(~cx, ~cy, ~rx, ~ry, ~paint, ~context);
      Option.iter(draw, fill);
      Option.iter(draw, stroke);

    | Line({x1, y1, x2, y2}) =>
      let x1 = userCoord(x1);
      let y1 = userCoord(y1);
      let x2 = userCoord(x2);
      let y2 = userCoord(y2);
      let draw = paint => line(~x1, ~y1, ~x2, ~y2, ~paint, ~context);
      Option.iter(draw, stroke);

    | Path({d}) =>
      let draw = paint => path(~d, ~paint, ~context);
      Option.iter(draw, stroke);

    | Polygon(_) => failwith("TODO")

    | Rect({x, y, width, height, rx, ry}) =>
      let x = userCoord(x);
      let y = userCoord(y);
      let width = userCoord(width);
      let height = userCoord(height);
      let rx = userCoord(rx);
      let ry = userCoord(ry);
      let draw = paint =>
        rect(~x, ~y, ~width, ~height, ~rx, ~ry, ~paint, ~context);
      Option.iter(draw, fill);
      Option.iter(draw, stroke);
    };
  };

  let rec element = context =>
    fun
    | Group(elements) => elements |> List.iter(element(context))
    | Geometry(shape) => geometry(context, shape)
    | Text(_) => ();

  let render = (~width, ~height, canvas, document: t) => {
    let context = {
      defs: document.defs,
      font: {
        family: "",
        size: 10.,
        xHeight: 10.,
        chWidth: 10.,
      },
      viewport: document.viewport,
      container: {
        width,
        height,
      },
      rootFontSize: 10.,
      canvas,
    };

    element(context, Group(document.elements));
  };
};
