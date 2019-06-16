open Revery;
open Revery.UI;
open Revery.UI.Components;

type todo = {
  id: int,
  task: string,
  isDone: bool,
};

module Filter = {
  type t =
    | All
    | Completed
    | NotCompleted;

  let show = (v: t) =>
    switch (v) {
    | All => "All"
    | Completed => "Completed"
    | NotCompleted => "NotCompleted"
    };
};

type state = {
  todos: list(todo),
  filter: Filter.t,
  inputValue: string,
  nextId: int,
};

type action =
  | AddTodo
  | ChangeFilter(Filter.t)
  | UpdateInputTextValue(string)
  | ToggleTaskState(int);

let reducer = (action: action, state: state) =>
  switch (action) {
  | AddTodo => {
      ...state,
      todos:
        List.append(
          state.todos,
          [{id: state.nextId, task: state.inputValue, isDone: false}],
        ),
      inputValue: "",
      nextId: state.nextId + 1,
    }
  | UpdateInputTextValue(text) => {...state, inputValue: text}
  | ToggleTaskState(id) =>
    let todos =
      List.map(
        item => item.id == id ? {...item, isDone: !item.isDone} : item,
        state.todos,
      );
    {...state, todos};
  | ChangeFilter(filter) => {...state, filter}
  };

module FilterSection = {
  let component = React.component("FilterSection");

  let createElement = (~children as _, ~currentFilter, ~onPickingFilter, ()) =>
    component(hooks =>
      (
        hooks,
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
            title={Filter.show(Filter.All)}
            color={
              switch (currentFilter) {
              | Filter.All => Colors.dodgerBlue
              | _ => Colors.lightSkyBlue
              }
            }
            onClick={() => onPickingFilter(Filter.All)}
          />
          <Button
            height=50
            width=150
            fontSize=15
            title={Filter.show(Completed)}
            color={
              switch (currentFilter) {
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
            title={Filter.show(NotCompleted)}
            color={
              switch (currentFilter) {
              | NotCompleted => Colors.dodgerBlue
              | _ => Colors.lightSkyBlue
              }
            }
            onClick={() => onPickingFilter(NotCompleted)}
          />
        </View>,
      )
    );
};

module Example = {
  let component = React.component("TodoMVC");

  let createElement = (~children as _, ()) =>
    component(hooks => {
      let ({todos, inputValue, filter, _}, dispatch, hooks) =
        Hooks.reducer(
          ~initialState={todos: [], filter: All, inputValue: "", nextId: 0},
          reducer,
          hooks,
        );

      let hooks =
        Hooks.effect(
          OnMount,
          () => {
            let unsubscribe = () => ();
            Some(unsubscribe);
          },
          hooks,
        );

      let renderTodo = task =>
        <View style=Style.[flexDirection(`Row)]>
          <Checkbox
            checked={task.isDone}
            onChange={() => dispatch(ToggleTaskState(task.id))}
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
      (
        hooks,
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
            currentFilter=filter
            onPickingFilter={filter => dispatch(ChangeFilter(filter))}
          />
          <View style=Style.[flexDirection(`Row)]>
            <Input
              style=Style.[width(400)]
              placeholder="Add your Todo here"
              value=inputValue
              onChange={({value, _}) =>
                dispatch(UpdateInputTextValue(value))
              }
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
        </View>,
      );
    });
};

let render = () => <Example />;
