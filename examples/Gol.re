open Core;
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

type action =
  | Tick
  | ToggleAlive(Position.t);

let blinker =
  Map.of_alist_exn(
    (module Position),
    [((5, 4), Alive), ((5, 5), Alive), ((5, 6), Alive)],
  );

let pulsar =
  Map.of_alist_exn(
    (module Position),
    [
      ((4, 2), Alive),
      ((5, 2), Alive),
      ((6, 2), Alive),
      ((10, 2), Alive),
      ((11, 2), Alive),
      ((12, 2), Alive),
      ((2, 4), Alive),
      ((7, 4), Alive),
      ((9, 4), Alive),
      ((14, 4), Alive),
      ((2, 5), Alive),
      ((7, 5), Alive),
      ((9, 5), Alive),
      ((14, 5), Alive),
      ((2, 6), Alive),
      ((7, 6), Alive),
      ((9, 6), Alive),
      ((14, 6), Alive),
      ((4, 7), Alive),
      ((5, 7), Alive),
      ((6, 7), Alive),
      ((10, 7), Alive),
      ((11, 7), Alive),
      ((12, 7), Alive),
      ((4, 9), Alive),
      ((5, 9), Alive),
      ((6, 9), Alive),
      ((10, 9), Alive),
      ((11, 9), Alive),
      ((12, 9), Alive),
      ((2, 10), Alive),
      ((7, 10), Alive),
      ((9, 10), Alive),
      ((14, 10), Alive),
      ((2, 11), Alive),
      ((7, 11), Alive),
      ((9, 11), Alive),
      ((14, 11), Alive),
      ((2, 12), Alive),
      ((7, 12), Alive),
      ((9, 12), Alive),
      ((14, 12), Alive),
      ((4, 14), Alive),
      ((5, 14), Alive),
      ((6, 14), Alive),
      ((10, 14), Alive),
      ((11, 14), Alive),
      ((12, 14), Alive),
    ],
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

module Row = {
  let component = React.component("Row");

  let make = children =>
    component((_slots: React.Hooks.empty) => {
      let style =
        Style.make(
          ~flexDirection=LayoutTypes.Row,
          ~alignItems=LayoutTypes.AlignStretch,
          ~justifyContent=LayoutTypes.JustifyCenter,
          ~backgroundColor=Colors.darkGrey,
          ~flexGrow=1,
          (),
        );
      <View style> ...children </View>;
    });

  let createElement = (~children, ()) => React.element(make(children));
};

module Column = {
  let component = React.component("Column");

  let make = children =>
    component((_slots: React.Hooks.empty) => {
      let style =
        Style.make(
          ~flexDirection=LayoutTypes.Column,
          ~alignItems=LayoutTypes.AlignStretch,
          ~justifyContent=LayoutTypes.JustifyCenter,
          ~backgroundColor=Colors.darkGrey,
          ~flexGrow=1,
          (),
        );
      <View style> ...children </View>;
    });
  let createElement = (~children, ()) => React.element(make(children));
};

module Button = {
  let component = React.component("Button");

  let make =
      (~fontFamily="Roboto-Regular.ttf", ~contents: string, ~onClick, ()) =>
    component((_slots: React.Hooks.empty) => {
      let clickableStyle =
        Style.make(
          ~position=LayoutTypes.Relative,
          ~backgroundColor=Colors.lightGrey,
          ~justifyContent=LayoutTypes.JustifyCenter,
          ~alignItems=LayoutTypes.AlignCenter,
          ~flexGrow=1,
          /* Min width */
          ~width=150,
          ~margin=10,
          (),
        );
      let viewStyle =
        Style.make(
          ~position=LayoutTypes.Relative,
          ~justifyContent=LayoutTypes.JustifyCenter,
          ~alignItems=LayoutTypes.AlignCenter,
          (),
        );
      let textStyle =
        Style.make(~color=Colors.black, ~fontFamily, ~fontSize=32, ());

      <Clickable style=clickableStyle onClick>
        <View style=viewStyle> <Text style=textStyle text=contents /> </View>
      </Clickable>;
    });

  let createElement =
      (
        ~fontFamily="Roboto-Regular.ttf",
        ~contents,
        ~onClick,
        ~children as _,
        (),
      ) => {
    React.element(make(~fontFamily, ~contents, ~onClick, ()));
  };
};

module Cell = {
  let component = React.component("Column");

  let make = (~cell, ~onClick, ()) =>
    component((_slots: React.Hooks.empty) => {
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
        | Alive => <View style=aliveStyle />
        | Dead => <View style=deadStyle />
        };
      <Clickable style=clickableStyle onClick> style </Clickable>;
    });
  let createElement = (~cell, ~onClick, ~children as _, ()) =>
    React.element(make(~cell, ~onClick, ()));
};

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
              y =>
                <Row>
                  <Cell cell=cell((x, y)) onClick={() => onClick((x, y))} />
                </Row>,
            rows,
          )}
        </Column>,
    cols,
  );
};

let reducer = (action, state) =>
  switch (action) {
  | Tick => {...state, universe: evolve(state.universe)}
  | ToggleAlive(position) => {
      ...state,
      universe:
        Map.change(state.universe, position, ~f=x =>
          switch (x) {
          | Some(Alive) => None
          | _ => Some(Alive)
          }
        ),
    }
  };

module GameOfLife = {
  let component = React.component("GameOfLife");

  let make = (~model: model, ()) =>
    component(slots => {
      let ({universe, viewPort}, dispatch, _slots: React.Hooks.empty) =
        React.Hooks.reducer(~initialState=model, reducer, slots);
      let onClick = pos => dispatch(ToggleAlive(pos));
      let controlsStyle =
        Style.make(~backgroundColor=Colors.white, ~height=120, ());
      <Column>
        <Row> ...{viewPortRender(viewPort, universe, onClick)} </Row>
        <View style=controlsStyle>
          <Button contents="Tick" onClick={_ => dispatch(Tick)} />
        </View>
      </Column>;
    });

  let createElement = (~model, ~children as _, ()) => {
    React.element(make(~model, ()));
  };
};

let render = () => {
  let viewPort = {xMin: 0, xMax: 20, yMin: 0, yMax: 20};
  let model = {viewPort, universe: pulsar};
  <GameOfLife model />;
};
