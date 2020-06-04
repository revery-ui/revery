type point = {
  x: float,
  y: float,
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
    | Polyline({points: list(point)})
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
  viewport: option(viewport),
};
