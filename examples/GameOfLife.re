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
  type t = (int, int);
};

module Universe =
  Map.Make({
    type t = Position.t;
    let compare = compare;
  });

let universeFromList = positions => {
  List.fold_left(
    (acc, pos) => Universe.add(pos, Alive, acc),
    Universe.empty,
    positions,
  );
};

let blinker = universeFromList([(5, 4), (5, 5), (5, 6)]);
let pulsar =
  universeFromList([
    (4, 2),
    (5, 2),
    (6, 2),
    (10, 2),
    (11, 2),
    (12, 2),
    (2, 4),
    (7, 4),
    (9, 4),
    (14, 4),
    (2, 5),
    (7, 5),
    (9, 5),
    (14, 5),
    (2, 6),
    (7, 6),
    (9, 6),
    (14, 6),
    (4, 7),
    (5, 7),
    (6, 7),
    (10, 7),
    (11, 7),
    (12, 7),
    (4, 9),
    (5, 9),
    (6, 9),
    (10, 9),
    (11, 9),
    (12, 9),
    (2, 10),
    (7, 10),
    (9, 10),
    (14, 10),
    (2, 11),
    (7, 11),
    (9, 11),
    (14, 11),
    (2, 12),
    (7, 12),
    (9, 12),
    (14, 12),
    (4, 14),
    (5, 14),
    (6, 14),
    (10, 14),
    (11, 14),
    (12, 14),
  ]);

let withDefault = (opt, ~default) =>
  switch (opt) {
  | Some(x) => x
  | None => default
  };

let findCell = (universe, position) => {
  Universe.find_opt(position, universe)
  |> withDefault(~default=Dead)
  |> (cell => (position, cell));
};

let numberOfLive = neighbours =>
  neighbours
  |> List.filter(x =>
       switch (x) {
       | Alive => true
       | _ => false
       }
     )
  |> List.length;

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

    if (numberOfLiveNeighbours == 2 || numberOfLiveNeighbours == 3) {
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
    if (numberOfLive(neighbours) == 3) {
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

  let reducedLifeCycle =
    actions |> List.filter((!=)(Same)) |> (l => List.nth_opt(l, 0));

  withDefault(reducedLifeCycle, ~default=Same);
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
      position => findCell(universe, position) |> snd,
      neighbourPositions(position),
    );
  let evolvedCell = applyRules(cell, neighbours);
  (position, evolvedCell);
};

let evolve = universe => {
  let find_relevant_cells = position =>
    neighbourPositions(position) |> List.map(findCell(universe));
  let keys = Universe.bindings(universe) |> List.map(fst);
  keys
  |> List.map(find_relevant_cells)
  |> List.concat
  |> List.sort_uniq(compare)
  |> List.map(evolveCell(universe))
  |> List.filter(x => snd(x) |> (==)(Alive))
  |> List.map(fst)
  |> universeFromList;
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

let viewPortRender = (viewPort, universe: Universe.t(cell), onClick) => {
  let cell = pos =>
    switch (Universe.find_opt(pos, universe)) {
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
    x =>
      <Column>
        ...{List.map(
          y =>
            <Row>
              <Cell cell={cell((x, y))} onClick={() => onClick((x, y))} />
            </Row>,
          rows,
        )}
      </Column>,
    cols,
  );
};

type model = {
  viewPort,
  universe: Universe.t(cell),
};

type action =
  | Tick
  | ToggleAlive(Position.t);

let reducer = (action, state) =>
  switch (action) {
  | Tick => {...state, universe: evolve(state.universe)}
  | ToggleAlive(position) => {
      ...state,
      universe:
        Universe.update(
          position,
          x =>
            switch (x) {
            | Some(Alive) => None
            | _ => Some(Alive)
            },
          state.universe,
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
