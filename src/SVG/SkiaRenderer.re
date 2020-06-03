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
  let scaleX = float(context.container.width) /. context.viewport.width;
  let scaleY = float(context.container.height) /. context.viewport.height;
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
  CanvasContext.concat(transform, context.canvas);

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

let render = (~width, ~height, document: Model.t, canvas) => {
  let context =
    RenderContext.{
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
