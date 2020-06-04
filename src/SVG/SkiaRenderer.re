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

open Revery_Core;
open Revery_Draw;
open Model;
open RenderContext;

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
  | `pc(_) => failwith("TODO - userCoord")
  | `percentage(_) => failwith("TODO - userCoord percentage")
  | `auto => failwith("TODO - userCoord auto");

let paint =
  fun
  | `none => None
  | `currentColor => failwith("TODO - currentColor")
  | `color(color) => {
      let paint = Skia.Paint.make();
      Skia.Paint.setAntiAlias(paint, true);
      Skia.Paint.setColor(paint, color);
      Some(paint);
    }
  | `funciri(_) => failwith("TODO - funciri")
  | `inherit_ => failwith("TODO - inherit");

let blackPaint = paint(`color(Colors.black |> Color.toSkia)) |> Option.get;

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
    {
      let previous = ref(None);

      command => {
        switch (command) {
        | `M(x, y) => Skia.Path.moveTo(path, x, y)
        | `m(dx, dy) => Skia.Path.rMoveTo(path, dx, dy)

        | `L(x, y) => Skia.Path.lineTo(path, x, y)
        | `l(dx, dy) => Skia.Path.rLineTo(path, dx, dy)

        | `H(x) =>
          let last = Skia.Point.make(0., 0.);
          if (Skia.Path.getLastPoint(path, last)) {
            Skia.Path.lineTo(path, x, Skia.Point.getY(last));
          };
        | `h(dx) => Skia.Path.rLineTo(path, dx, 0.)

        | `V(y) =>
          let last = Skia.Point.make(0., 0.);
          if (Skia.Path.getLastPoint(path, last)) {
            Skia.Path.lineTo(path, Skia.Point.getX(last), y);
          };
        | `v(dy) => Skia.Path.rLineTo(path, 0., dy)

        | `C(x1, y1, x2, y2, x, y) =>
          Skia.Path.cubicTo(path, x1, y1, x2, y2, x, y)
        | `c(dx1, dy1, dx2, dy2, dx, dy) =>
          Skia.Path.rCubicTo(path, dx1, dy1, dx2, dy2, dx, dy)

        | `S(x2, y2, x, y) =>
          let (x1, y1) =
            switch (previous^) {
            | Some(`C(_, _, px2, py2, px, py) | `S(px2, py2, px, py)) => (
                px -. (px2 -. px),
                py -. (py2 -. py),
              )
            | _ => (x, y)
            };
          Skia.Path.cubicTo(path, x1, y1, x2, y2, x, y);
        | `s(dx2, dy2, dx, dy) => failwith("TODO - path s")

        | `Q(x1, y1, x, y) => Skia.Path.quadTo(path, x1, y1, x, y)
        | `q(dx1, dy1, dx, dy) => Skia.Path.rQuadTo(path, dx1, dy1, dx, dy)

        | `T(_) => failwith("TODO - path T")
        | `t(_) => failwith("TODO - path t")

        | `A(rx, ry, angle, size, sweep, x, y) =>
          Skia.Path.arcTo(path, rx, ry, angle, size, sweep, x, y)
        | `a(rx, ry, angle, size, sweep, dx, dy) =>
          Skia.Path.rArcTo(path, rx, ry, angle, size, sweep, dx, dy)

        | `Z
        | `z => Skia.Path.close(path)
        };
        previous := Some(command);
      };
    },
    d,
  );
  
  CanvasContext.drawPath(~path, ~paint, context.canvas);
};

let polygon = (~points, ~paint, ~context) => {
  switch (points) {
  | [] => ()
  | [first, ...rest] =>
    let path = Skia.Path.make();

    Skia.Path.moveTo(path, first.x, first.y);
    List.iter(({x, y}) => Skia.Path.lineTo(path, x, y), rest);
    Skia.Path.close(path);

    CanvasContext.drawPath(~path, ~paint, context.canvas);
  };
};

let polyline = (~points, ~paint, ~context) => {
  switch (points) {
  | [] => ()
  | [first, ...rest] =>
    let path = Skia.Path.make();

    Skia.Path.moveTo(path, first.x, first.y);
    List.iter(({x, y}) => Skia.Path.lineTo(path, x, y), rest);

    CanvasContext.drawPath(~path, ~paint, context.canvas);
  };
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
  let fill =
    List.find_map(
      fun
      | `fill(spec) => Some(paint(spec))
      | _ => None,
      shape.attributes,
    )
    |> Option.value(~default=Some(blackPaint));

  let stroke = {
    let make = spec => {
      switch (paint(spec)) {
      | Some(paint) =>
        Skia.Paint.setStyle(paint, Stroke);
        Skia.Paint.setStrokeWidth(paint, 1.);

        List.iter(
          fun
          | `strokeWidth(width) =>
            Skia.Paint.setStrokeWidth(paint, userCoord(width))
          | _ => (),
          shape.attributes,
        );

        Some(paint);
      | None => None
      };
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
    Option.iter(draw, fill);
    Option.iter(draw, stroke);

  | Polygon({points}) =>
    let draw = paint => polygon(~points, ~paint, ~context);
    Option.iter(draw, fill);
    Option.iter(draw, stroke);

  | Polyline({points}) =>
    let draw = paint => polyline(~points, ~paint, ~context);
    Option.iter(draw, fill);
    Option.iter(draw, stroke);

  | Rect({x, y, width, height, rx, ry}) =>
    let x = userCoord(x);
    let y = userCoord(y);
    let width = userCoord(width);
    let height = userCoord(height);
    let (rx, ry) =
      switch (rx, ry) {
      | (`auto, `auto) => (0., 0.)
      | (`auto, r)
      | (r, `auto) =>
        let r = userCoord(r);
        (r, r);
      | (rx, ry) => (userCoord(rx), userCoord(ry))
      };
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
  | Text(_) => failwith("TODO - text");

let applyViewportTransform =
    (~width, ~height, viewport: Model.viewport, canvas) => {
  let scaleX = float(width) /. viewport.width;
  let scaleY = float(height) /. viewport.height;
  let translateX = viewport.origin.x *. scaleX;
  let translateY = viewport.origin.y *. scaleY;

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
  CanvasContext.concat(transform, canvas);
};

let render = (~width, ~height, document: Model.t, canvas) => {
  let viewport =
    switch (document.viewport) {
    | Some(viewport) =>
      applyViewportTransform(~width, ~height, viewport, canvas);
      viewport;

    | None => {
        origin: {
          x: 0.,
          y: 0.,
        },
        width: float(width),
        height: float(height),
      }
    };

  let context =
    RenderContext.{
      defs: document.defs,
      font: {
        family: "",
        size: 10.,
        xHeight: 10.,
        chWidth: 10.,
      },
      viewport,
      rootFontSize: 10.,
      canvas,
    };

  element(context, Group(document.elements));
};
