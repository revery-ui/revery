open Core;
open Revery;
open Revery.Core;
open Revery.UI;

type viewPort = {
  xMin: int,
  yMin: int,
  xMax: int,
  yMax: int,
};

type cell =
  | Alive
  | Dead;

module Position = {
  module T = {
    type t = (int, int);
    let compare = Tuple2.compare(~cmp1=Int.compare, ~cmp2=Int.compare);
    let sexp_of_t = Tuple2.sexp_of_t(Int.sexp_of_t, Int.sexp_of_t);
    let t_of_sexp = Tuple2.t_of_sexp(Int.t_of_sexp, Int.t_of_sexp);
  };

  include T;
  include Comparable.Make(T);
};

let blinker =
  Map.of_alist_exn(
    (module Position),
    [((1, 1), Alive), ((2, 1), Alive), ((3, 1), Alive)],
  );

module Row = (
  val component((render, ~children, ()) =>
        render(
          () => {
            let style =
              Style.make(
                ~flexDirection=LayoutTypes.Row,
                ~alignItems=LayoutTypes.AlignStretch,
                ~justifyContent=LayoutTypes.JustifyCenter,
                ~backgroundColor=Colors.darkGrey,
                ~flexGrow=1,
                (),
              );
            <view style> ...children </view>;
          },
          ~children,
        )
      )
);

module Column = (
  val component((render, ~children, ()) =>
        render(
          () => {
            let style =
              Style.make(
                ~flexDirection=LayoutTypes.Column,
                ~alignItems=LayoutTypes.AlignStretch,
                ~justifyContent=LayoutTypes.JustifyCenter,
                ~backgroundColor=Colors.darkGrey,
                ~flexGrow=1,
                (),
              );
            <view style> ...children </view>;
          },
          ~children,
        )
      )
);

module Cell = (
  val component((render, ~cell, ()) =>
        render(() => {
          let border = Style.Border.make(~color=Colors.gray, ~width=2, ());
          let baseStyle =
            Style.make(
              ~flexDirection=LayoutTypes.Column,
              ~alignItems=LayoutTypes.AlignStretch,
              ~justifyContent=LayoutTypes.JustifyCenter,
              ~border,
              ~flexGrow=1,
              (),
            );
          let aliveStyle =
            Style.extend(baseStyle, ~backgroundColor=Colors.red, ());
          let deadStyle =
            Style.extend(baseStyle, ~backgroundColor=Colors.black, ());
          switch (cell) {
          | Alive => <view style=aliveStyle />
          | Dead => <view style=deadStyle />
          };
        })
      )
);

let viewPortRender = (viewPort, universe) => {
  let cell = pos =>
    switch (Map.find(universe, pos)) {
    | Some(_) => Alive
    | None => Dead
    };
  let rec range = (min, max, result) =>
    if (min == max) {
      result;
    } else {
      range(min, max - 1, [max, ...result]);
    };
  let cols = range(viewPort.xMin, viewPort.xMax, []);
  let rows = range(viewPort.yMin, viewPort.yMax, []);
  List.map(
    ~f=
      x =>
        <Column>
          ...{List.map(
            ~f=
              y => {
                let foo = cell((x, y));
                <Row> <Cell cell=foo /> </Row>;
              },
            rows,
          )}
        </Column>,
    cols,
  );
};

let universe = blinker;

module GameOfLife = (
  val component((render, ~viewPort, ~children, ()) =>
        render(
          () => <Row> ...{viewPortRender(viewPort, universe)} </Row>,
          ~children,
        )
      )
);

let init = app => {
  let window = App.createWindow(app, "Revery GameOfLife");
  let viewPort = {xMin: 0, xMax: 10, yMin: 0, yMax: 10};

  let render = () => {
    <GameOfLife viewPort />;
  };

  UI.start(window, render);
};

App.start(init);
