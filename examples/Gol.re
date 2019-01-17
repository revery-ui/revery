open Core;
open Revery;
open Revery.Core;
open Revery.UI;
open Revery.UI.Components;

type viewPort = {
  xMin: int,
  yMin: int,
  xMax: int,
  yMax: int,
};

type cell =
  | Alive
  | Dead;

type action =
  | Tick;

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

type universe = Map.t(Position.t, cell, Position.comparator_witness);

type model = {
  viewPort,
  universe,
};

let blinker =
  Map.of_alist_exn(
    (module Position),
    [((5, 4), Alive), ((5, 5), Alive), ((5, 6), Alive)],
  );
let findCell = (universe, position) => {
  Map.find(universe, position)
  |> Option.value(~default=Dead)
  |> (cell => Tuple2.create(position, cell));
};

let numberOfLive = neighbours =>
  neighbours |> List.filter(~f=phys_equal(Alive)) |> List.length;

type lifeCycle =
  | Dies
  | Revives
  | Same;
let underPopulationRule = (cell, neighbours) =>
  switch (cell) {
  | Alive =>
    if (numberOfLive(neighbours) < 2) {
      Dies;
    } else {
      Same;
    }

  | Dead => Same
  };

let livesOnRule = (cell, neighbours) =>
  switch (cell) {
  | Alive =>
    let numberOfLiveNeighbours = numberOfLive(neighbours);

    if (phys_equal(numberOfLiveNeighbours, 2)
        || phys_equal(numberOfLiveNeighbours, 3)) {
      Same;
    } else {
      Dies;
    };

  | Dead => Same
  };

let overPopulationRule = (cell, neighbours) =>
  switch (cell) {
  | Alive =>
    if (numberOfLive(neighbours) > 3) {
      Dies;
    } else {
      Same;
    }

  | Dead => Same
  };

let reproductionRule = (cell, neighbours) =>
  switch (cell) {
  | Alive => Same

  | Dead =>
    if (phys_equal(numberOfLive(neighbours), 3)) {
      Revives;
    } else {
      Same;
    }
  };

let reduceLifeCycle = (cell, neighbours) => {
  let actions = [
    underPopulationRule(cell, neighbours),
    livesOnRule(cell, neighbours),
    overPopulationRule(cell, neighbours),
    reproductionRule(cell, neighbours),
  ];

  let reducedLifeCycle = actions |> List.filter(~f=(!=)(Same)) |> List.hd;

  Option.value(reducedLifeCycle, ~default=Same);
};

let applyRules = (cell, neighbours) => {
  let action = reduceLifeCycle(cell, neighbours);
  switch (action) {
  | Dies => Dead
  | Revives => Alive
  | Same => cell
  };
};

let neighbourPositions = ((x, y)) => [
  (x - 1, y - 1),
  (x, y - 1),
  (x + 1, y - 1),
  (x - 1, y),
  (x + 1, y),
  (x - 1, y + 1),
  (x, y + 1),
  (x + 1, y + 1),
];
let evolveCell = (universe, (position, cell)) => {
  let neighbours =
    List.map(
      ~f=position => findCell(universe, position) |> snd,
      neighbourPositions(position),
    );
  let evolvedCell = applyRules(cell, neighbours);
  (position, evolvedCell);
};

let evolve = universe => {
  let find_relevant_cells = position =>
    neighbourPositions(position) |> List.map(~f=findCell(universe));
  let compare = ((p1, _), (p2, _)) =>
    Tuple2.compare(~cmp1=Int.compare, ~cmp2=Int.compare, p1, p2);
  universe
  |> Map.keys
  |> List.map(~f=find_relevant_cells)
  |> List.concat
  |> List.dedup_and_sort(~compare)
  |> List.map(~f=evolveCell(universe))
  |> List.filter(~f=x => snd(x) |> phys_equal(Alive))
  |> Map.of_alist_exn((module Position));
};

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
  val component((render, ~cell, ~onClick, ()) =>
        render(() => {
          let border = Style.Border.make(~color=Colors.gray, ~width=2, ());
          let clickableStyle =
            Style.make(
              ~position=LayoutTypes.Relative,
              ~backgroundColor=Colors.transparentWhite,
              ~justifyContent=LayoutTypes.JustifyCenter,
              ~alignItems=LayoutTypes.AlignStretch,
              ~flexGrow=1,
              ~margin=0,
              (),
            );
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
          let style =
            switch (cell) {
            | Alive => <view style=aliveStyle />
            | Dead => <view style=deadStyle />
            };
          <Clickable style=clickableStyle onClick> style </Clickable>;
        })
      )
);

let viewPortRender = (viewPort, universe: universe, onClick) => {
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
                <Row> <Cell cell=foo onClick /> </Row>;
              },
            rows,
          )}
        </Column>,
    cols,
  );
};

let reducer = (state, action) =>
  switch (action) {
  | Tick => {...state, universe: evolve(state.universe)}
  };

module GameOfLife = (
  val component((render, ~model, ~children, ()) =>
        render(
          () => {
            let ({universe, viewPort}, dispatch) =
              useReducer(reducer, model);
            let onClick = _ => dispatch(Tick);
            <Row> ...{viewPortRender(viewPort, universe, onClick)} </Row>;
          },
          ~children,
        )
      )
);

let init = app => {
  let window = App.createWindow(app, "Revery GameOfLife");
  let viewPort = {xMin: 0, xMax: 10, yMin: 0, yMax: 10};
  let model = {viewPort, universe: blinker};

  let render = () => {
    <GameOfLife model />;
  };

  UI.start(window, render);
};

App.start(init);
