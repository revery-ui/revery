open Revery;
open Revery.Time;
open Revery.UI;
open Revery.UI.Components;

module ViewPort = {
  type t = {
    xMin: int,
    yMin: int,
    xMax: int,
    yMax: int,
  };
  type direction =
    | North
    | East
    | South
    | West;

  let create = size => {xMin: 0, xMax: size, yMin: 0, yMax: size};

  let changeDirection = (viewPort, direction) =>
    switch (direction) {
    | North => {...viewPort, yMin: viewPort.yMin - 1, yMax: viewPort.yMax - 1}
    | East => {...viewPort, xMin: viewPort.xMin + 1, xMax: viewPort.xMax + 1}
    | South => {...viewPort, yMin: viewPort.yMin + 1, yMax: viewPort.yMax + 1}
    | West => {...viewPort, xMin: viewPort.xMin - 1, xMax: viewPort.xMax - 1}
    };

  let zoomOut = viewPort => {
    xMin: viewPort.xMin - 1,
    xMax: viewPort.xMax + 1,
    yMin: viewPort.yMin - 1,
    yMax: viewPort.yMax + 1,
  };

  let zoomIn = viewPort =>
    if (viewPort.xMax > viewPort.xMin + 2) {
      {
        xMin: viewPort.xMin + 1,
        xMax: viewPort.xMax - 1,
        yMin: viewPort.yMin + 1,
        yMax: viewPort.yMax - 1,
      };
    } else {
      viewPort;
    };
};

module Position = {
  type t = (int, int);
};

type cell =
  | Alive
  | Dead;

module Universe = {
  module T =
    Map.Make({
      type t = Position.t;
      let compare = compare;
    });
  include T;
  let universeFromList = positions =>
    List.fold_left(
      (acc, pos) => T.add(pos, Alive, acc),
      T.empty,
      positions,
    );
};

module Examples = {
  let blinker = Universe.universeFromList([(5, 4), (5, 5), (5, 6)]);
  let pulsar =
    Universe.universeFromList([
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
};

module GameOfLife = {
  /* helper function for options */
  let withDefault = (opt, ~default) =>
    switch (opt) {
    | Some(x) => x
    | None => default
    };

  let findCell = (universe, position) =>
    Universe.find_opt(position, universe)
    |> withDefault(~default=Dead)
    |> (cell => (position, cell));

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
    |> Universe.universeFromList;
  };
};

module Row = {
  let component = React.component("Row");

  let style =
    Style.[
      flexDirection(`Row),
      alignItems(`Stretch),
      justifyContent(`Center),
      backgroundColor(Colors.darkGrey),
      flexGrow(1),
    ];

  let createElement = (~children, ()) =>
    component(hooks => (hooks, <View style> ...children </View>));
};

module Column = {
  let component = React.component("Column");

  let style =
    Style.[
      flexDirection(`Column),
      alignItems(`Stretch),
      justifyContent(`Center),
      flexGrow(1),
    ];

  let createElement = (~children, ()) =>
    component(hooks => (hooks, <View style> ...children </View>));
};

module Cell = {
  let component = React.component("Column");

  let clickableStyle =
    Style.[
      position(`Relative),
      backgroundColor(Colors.transparentWhite),
      justifyContent(`Center),
      alignItems(`Stretch),
      flexGrow(1),
      margin(0),
    ];
  let baseStyle =
    Style.[
      flexDirection(`Column),
      alignItems(`Stretch),
      justifyContent(`Center),
      flexGrow(1),
    ];
  let aliveStyle =
    Style.(merge(~source=baseStyle, ~target=[backgroundColor(Colors.red)]));
  let deadStyle =
    Style.(
      merge(~source=baseStyle, ~target=[backgroundColor(Colors.black)])
    );

  let createElement = (~cell, ~onClick, ~children as _, ()) =>
    component(hooks => {
      let style =
        switch (cell) {
        | Alive => <View style=aliveStyle />
        | Dead => <View style=deadStyle />
        };
      (hooks, <Clickable style=clickableStyle onClick> style </Clickable>);
    });
};

let viewPortRender =
    (viewPort: ViewPort.t, universe: Universe.t(cell), onClick) => {
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

type state = {
  viewPort: ViewPort.t,
  universe: Universe.t(cell),
  isRunning: bool,
  dispose,
}
and dispose = unit => unit;

type action =
  | TimerTick(Time.t)
  | StartTimer(dispose)
  | StopTimer
  | ToggleAlive(Position.t)
  | MoveViewPort(ViewPort.direction)
  | ZoomViewPort(zoom)
and zoom =
  | ZoomIn
  | ZoomOut;

let noop = () => ();
let reducer = (action, state) =>
  switch (action) {
  | TimerTick(_t) => {...state, universe: GameOfLife.evolve(state.universe)}
  | StartTimer(dispose) => {...state, dispose, isRunning: true}
  | StopTimer =>
    state.dispose();
    {...state, dispose: noop, isRunning: false};
  | ToggleAlive(position) => {
      ...state,
      universe:
        Universe.update(
          position,
          cell =>
            switch (cell) {
            | Some(Alive) => None
            | _ => Some(Alive)
            },
          state.universe,
        ),
    }
  | MoveViewPort(direction) => {
      ...state,
      viewPort: ViewPort.changeDirection(state.viewPort, direction),
    }
  | ZoomViewPort(zoom) => {
      ...state,
      viewPort:
        switch (zoom) {
        | ZoomIn => ViewPort.zoomIn(state.viewPort)
        | ZoomOut => ViewPort.zoomOut(state.viewPort)
        },
    }
  };

module GameOfLiveComponent = {
  let component = React.component("GameOfLiveComponent");

  let controlsStyle = Style.[height(120), flexDirection(`Row)];

  let createElement = (~state, ~children as _, ()) =>
    component(hooks => {
      let (state, dispatch, hooks) =
        Hooks.reducer(~initialState=state, reducer, hooks);

      let hooks =
        Hooks.effect(OnMount, () => Some(() => dispatch(StopTimer)), hooks);

      let toggleAlive = pos => dispatch(ToggleAlive(pos));

      let startStop = () =>
        state.isRunning
          ? dispatch(StopTimer)
          : {
            let dispose =
              Tick.interval(t => dispatch(TimerTick(t)), Seconds(0.));
            dispatch(StartTimer(dispose));
          };

      (
        hooks,
        <Column>
          <Row>
            ...{viewPortRender(state.viewPort, state.universe, toggleAlive)}
          </Row>
          <View style=controlsStyle>
            <Button
              fontFamily="FontAwesome5FreeSolid.otf"
              title={state.isRunning ? {||} : {||}}
              onClick=startStop
            />
            <Button
              fontFamily="FontAwesome5FreeSolid.otf"
              title={||}
              onClick={_ => dispatch(MoveViewPort(North))}
            />
            <Button
              fontFamily="FontAwesome5FreeSolid.otf"
              title={||}
              onClick={_ => dispatch(MoveViewPort(South))}
            />
            <Button
              fontFamily="FontAwesome5FreeSolid.otf"
              title={||}
              onClick={_ => dispatch(MoveViewPort(East))}
            />
            <Button
              fontFamily="FontAwesome5FreeSolid.otf"
              title={||}
              onClick={_ => dispatch(MoveViewPort(West))}
            />
            <Button
              fontFamily="FontAwesome5FreeSolid.otf"
              title={||}
              onClick={_ => dispatch(ZoomViewPort(ZoomIn))}
            />
            <Button
              fontFamily="FontAwesome5FreeSolid.otf"
              title={||}
              onClick={_ => dispatch(ZoomViewPort(ZoomOut))}
            />
          </View>
        </Column>,
      );
    });
};

let render = () => {
  let viewPort = ViewPort.create(15);
  let state = {
    viewPort,
    universe: Examples.pulsar,
    isRunning: false,
    dispose: noop,
  };
  <GameOfLiveComponent state />;
};
