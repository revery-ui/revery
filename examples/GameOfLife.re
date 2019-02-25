open Revery.Core;
open Revery.Time;
open Revery.UI;
open Revery.UI.Components;

module ViewPort = {

  type dimensions = {
    width: int,
    height: int,
  };

  type t = {
    posX: int,
    posY: int,
    dimensions: dimensions,
    cellSize: int,
  };
  type direction =
    | North
    | East
    | South
    | West;

  let create = () => {posX: 0, posY: 0, dimensions: {width: 1, height: 1}, cellSize: 16};

  let changeDirection = (viewPort, direction) =>
    switch (direction) {
    | North => {...viewPort, posY: viewPort.posX - 1 }
    | East => {...viewPort, posX: viewPort.posX + 1 }
    | South => {...viewPort, posY: viewPort.posY - 1}
    | West => {...viewPort, posX: viewPort.posX - 1}
    };

  let setSize = (viewPort, dimensions) => {
    ...viewPort,
    dimensions,
  };

  let zoomOut = viewPort => {
      ...viewPort,
      cellSize: max(1, viewPort.cellSize / 2)
  };

  let zoomIn = viewPort => {
      ...viewPort,
      cellSize: viewPort.cellSize * 2,
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

  let clickableStyle =
    Style.[
      position(`Relative),
      backgroundColor(Colors.transparentWhite),
      justifyContent(`Center),
      alignItems(`Stretch),
      flexGrow(1),
      margin(0),
    ];
  let baseStyle = (x, y, size, alive) => 
    Style.[
      position(`Absolute),
      layoutMode(`Minimal),
      width(size),
      height(size),
      left(x),
      top(y),
      backgroundColor(alive ? Colors.red : Colors.black),
      /* flexDirection(`Column), */
      /* alignItems(`Stretch), */
      /* justifyContent(`Center), */
      /* flexGrow(1), */
    ];


module Cell = {
  let component = React.component("Column");

  let createElement = (~x, ~y, ~size, ~cell, ~onClick as _, ~children as _, ()) =>
    component(hooks => {
        let cell = switch (cell) {
        | Alive => <View style=baseStyle(x, y, size, true) />
        | Dead => <View style=baseStyle(x, y, size, false) />
        };
      (hooks, cell);
    });
};

let viewPortRender =
    (viewPort: ViewPort.t, universe: Universe.t(cell), onClick) => {
  let cell = pos =>
    switch (Universe.find_opt(pos, universe)) {
    | Some(_) => Alive
    | None => Dead
    };
  /* let rec range = (min, max, result) => */
  /*   if (min == max) { */
  /*     result; */
  /*   } else { */
  /*     range(min, max - 1, [max, ...result]); */
  /*   }; */
  let maxY = viewPort.dimensions.width / viewPort.cellSize;
  let maxX = viewPort.dimensions.height / viewPort.cellSize;

  let x = ref(0);
  let y = ref(0);

  let cells: ref(list(React.syntheticElement)) = ref([]);

  let cellSize = viewPort.cellSize;

  while (x^ < maxX) {
      y := 0;
    while (y^ < maxY) {

        let xVal = x^;
        let yVal = y^;

        let newCell = <Cell x={xVal * cellSize} y={yVal * cellSize} size={cellSize} cell=cell((xVal + viewPort.posX, yVal + viewPort.posY)) onClick=onClick((xVal, yVal)) />;

        cells := [newCell, ...cells^];

        incr(y);
    };

    incr(x);
  };


  cells^
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
  | SetViewPortSize(ViewPort.dimensions)
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
  | SetViewPortSize(dimensions) => {
    ...state,
    viewPort: ViewPort.setSize(state.viewPort, dimensions)
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
        React.Hooks.reducer(~initialState=state, reducer, hooks);

      let hooks =
        React.Hooks.effect(
          OnMount,
          () => Some(() => dispatch(StopTimer)),
          hooks,
        );

      let toggleAlive = (pos, ()) => dispatch(ToggleAlive(pos));

      let startStop = () =>
        state.isRunning
          ? dispatch(StopTimer)
          : {
            let dispose =
              Tick.interval(t => dispatch(TimerTick(t)), Seconds(0.));
            dispatch(StartTimer(dispose));
          };

      let onDimensionsChanged = ({width, height, _}: NodeEvents.DimensionsChangedEventParams.t ) => {
          let action = SetViewPortSize({width, height});
          dispatch(action);
        print_endline ("ON DIMENSIONS CHANGED: width " ++ string_of_int(width) ++ " height: " ++ string_of_int(height));
      };

      (
        hooks,
        <Column>
          <Row>
            <View style={Style.[flexGrow(1), backgroundColor(Colors.red)]} onDimensionsChanged>
            ...{viewPortRender(state.viewPort, state.universe, toggleAlive)}
            </View>
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
  let viewPort = ViewPort.create();
  let state = {
    viewPort,
    universe: Examples.pulsar,
    isRunning: false,
    dispose: noop,
  };
  <GameOfLiveComponent state />;
};
