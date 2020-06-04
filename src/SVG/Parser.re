open SimpleXml;
open Model;

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

let coordinates = str => {
  let buffer = Buffer.create(10);
  let acc = ref([]);

  let accept = () =>
    if (Buffer.length(buffer) > 0) {
      let coord = Buffer.contents(buffer) |> float_of_string;
      acc := [coord, ...acc^];
      Buffer.clear(buffer);
    };

  String.iter(
    fun
    | ('0'..'9' | '.' | '+' | '-') as ch => Buffer.add_char(buffer, ch)
    | '\t'
    | ' '
    | '\n'
    | '\012'
    | '\r'
    | ',' => accept()
    | ch => failwith("invalid character in path data: " ++ Char.escaped(ch)),
    str,
  );

  accept();

  List.rev(acc^);
};

let pathCommands = str => {
  let tokens = {
    let buffer = Buffer.create(10);
    let acc = ref([]);

    let acceptArg = () =>
      if (Buffer.length(buffer) > 0) {
        let token = Buffer.contents(buffer) |> float_of_string;
        acc := [`arg(token), ...acc^];
        Buffer.clear(buffer);
      };

    String.iter(
      fun
      // argument
      | ('0'..'9' | '.' | '+' | '-') as ch => Buffer.add_char(buffer, ch)
      // white-space or comma
      | '\t'
      | ' '
      | '\n'
      | '\012'
      | '\r'
      | ',' => acceptArg()
      // commands
      | (
          'M' | 'm' | 'L' | 'l' | 'H' | 'h' | 'V' | 'v' | 'C' | 'c' | 'S' | 's' |
          'Q' |
          'q' |
          'T' |
          't' |
          'A' |
          'a' |
          'Z' |
          'z'
        ) as ch => {
          acceptArg();
          acc := [`cmd(ch), ...acc^];
        }
      | ch =>
        failwith("invalid character in path data: " ++ Char.escaped(ch)),
      str,
    );

    acceptArg();

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
        `arg(size),
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
            size == 0. ? `large : `small,
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
  List.assoc_opt(key, attrs) |> Option.map(length) |> Option.value(~default);

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

let attr_viewBox = attrs =>
  Option.bind(List.assoc_opt("viewBox", attrs), str =>
    switch (coordinates(str)) {
    | [x, y, width, height] => Some({
                                  origin: {
                                    x,
                                    y,
                                  },
                                  width,
                                  height,
                                })
    | _ => None
    }
  );

let attr_points = (key, ~default, attrs) => {
  let rec coordsToPoints = acc =>
    fun
    | []
    | [_] => List.rev(acc)
    | [x, y, ...rest] => coordsToPoints([{x, y}, ...acc], rest);

  List.assoc_opt(key, attrs)
  |> Option.map(coordinates)
  |> Option.map(coordsToPoints([]))
  |> Option.value(~default);
};

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

let polygon = attrs =>
  geometry(
    Polygon({points: attr_points("points", ~default=[], attrs)}),
    attrs,
  );

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
  | Element("ellipse", attrs, _children) => Some(Geometry(ellipse(attrs)))
  | Element("line", attrs, _children) => Some(Geometry(line(attrs)))
  | Element("path", attrs, _children) => Some(Geometry(path(attrs)))
  | Element("polygon", attrs, _children) => Some(Geometry(polygon(attrs)))
  | Element("rect", attrs, _children) => Some(Geometry(rect(attrs)))
  | _ => None;

let svg =
  fun
  | Element("svg", attrs, children) => {
      defs: [],
      elements: List.filter_map(element, children),
      viewport: attr_viewBox(attrs),
    }
  | _ => failwith("svg root element expected");
