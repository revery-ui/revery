open Revery.UI;
open Revery.Core;
open Revery.UI.Components;

type todo = {
  id: int,
  task: string,
  isDone: bool,
};

type filter =
  | All
  | Completed
  | NotCompleted;

let textOfFilter = (filter: filter) => {
  switch (filter) {
  | All => "All"
  | Completed => "Completed"
  | NotCompleted => "NotCompleted"
  };
};

type state = {
  todos: list(todo),
  filter,
  inputValue: string,
  nextId: int,
};

type action =
  | AddTodo
  | ChangeFilter(filter)
  | UpdateInputTextValue(string)
  | ChangeTaskState(int, bool);

let reducer = (action: action, state: state) => {
  switch (action) {
  | AddTodo => {
      ...state,
      todos: [
        {id: state.nextId, task: state.inputValue, isDone: false},
        ...state.todos,
      ],
      nextId: state.nextId + 1,
    }
  | UpdateInputTextValue(text) => {...state, inputValue: text}
  | ChangeTaskState(id, isDone) =>
    let todos =
      List.map(
        item => item.id == id ? {...item, isDone} : item,
        state.todos,
      );
    {...state, todos};
  | ChangeFilter(filter) => {...state, filter}
  };
};

module FilterSection = {
  let component = React.component("FilterSection");

  let make = (_children, currentFilier, onPickingFilter) =>
    component((_slots: React.Hooks.empty) =>
      <View
        style=Style.[
          flexDirection(`Row),
          width(500),
          alignItems(`Center),
          justifyContent(`Center),
        ]>
        <Button
          height=50
          width=150
          fontSize=15
          title="All"
          color={
            switch (currentFilier) {
            | All => Colors.dodgerBlue
            | _ => Colors.lightSkyBlue
            }
          }
          onClick={() => onPickingFilter(All)}
        />
        <Button
          height=50
          width=150
          fontSize=15
          title="Completed"
          color={
            switch (currentFilier) {
            | Completed => Colors.dodgerBlue
            | _ => Colors.lightSkyBlue
            }
          }
          onClick={() => onPickingFilter(Completed)}
        />
        <Button
          height=50
          width=150
          fontSize=15
          title="Not Completed"
          color={
            switch (currentFilier) {
            | NotCompleted => Colors.dodgerBlue
            | _ => Colors.lightSkyBlue
            }
          }
          onClick={() => onPickingFilter(NotCompleted)}
        />
      </View>
    );

  let createElement = (~children, ~currentFilier, ~onPickingFilter, ()) =>
    React.element(make(children, currentFilier, onPickingFilter));
};

module Example = {
  let component = React.component("TodoMVC");

  let make = window =>
    component(slots => {
      let ({todos, inputValue, filter, _}, dispatch, slots) =
        React.Hooks.reducer(
          ~initialState={
            todos: [],
            filter: All,
            inputValue: "",
            nextId: 0,
          },
          reducer,
          slots,
        );

      let _slots: React.Hooks.empty =
        React.Hooks.effect(
          OnMount,
          () => {
            let unsubscribe = () => ();
            Some(unsubscribe);
          },
          slots,
        );

      let renderTodo = task => {
        <View style=Style.[flexDirection(`Row)]>
          <Checkbox
            checked={task.isDone}
            onChange={checked => dispatch(ChangeTaskState(task.id, checked))}
          />
          <Text
            style=Style.[
              color(Colors.black),
              fontFamily("Roboto-Regular.ttf"),
              fontSize(20),
              margin(4),
            ]
            text={task.task}
          />
        </View>;
      };

      let filteredList =
        List.filter(
          task =>
            switch (filter) {
            | All => true
            | Completed => task.isDone
            | NotCompleted => !task.isDone
            },
          todos,
        );

      let listOfTodos = List.map(renderTodo, filteredList);
      <View
        style=Style.[
          position(`Absolute),
          top(0),
          bottom(0),
          left(0),
          right(0),
          alignItems(`Center),
          justifyContent(`Center),
          flexDirection(`Column),
          backgroundColor(Colors.white),
        ]>
        <FilterSection
          currentFilier=filter
          onPickingFilter={filter => dispatch(ChangeFilter(filter))}
        />
        <View style=Style.[flexDirection(`Row)]>
          <Input
            style=Style.[width(400)]
            window
            placeholder="Add your Todo here"
            onChange={(~value) => dispatch(UpdateInputTextValue(value))}
          />
          <Button
            width=50
            height=50
            disabled={
              switch (inputValue) {
              | "" => true
              | _ => false
              }
            }
            title="+"
            onClick={() => dispatch(AddTodo)}
          />
        </View>
        <ScrollView
          style=Style.[
            height(200),
            width(450),
            border(~width=1, ~color=Colors.black),
          ]>
          <View> ...listOfTodos </View>
        </ScrollView>
      </View>;
    });

  let createElement = (~window, ~children as _, ()) =>
    React.element(make(window));
};

let render = window => <Example window />;