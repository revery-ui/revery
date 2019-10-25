open Revery;
open Revery.UI;
open Revery.UI.Components;

module Constants = {
  let fontSize = 12;
}

module Filter = {
  type t =
    | All
    | Completed
    | NotCompleted;

  let show = (v: t) =>
    switch (v) {
    | All => "All"
    | Completed => "Completed"
    | NotCompleted => "Not Completed"
    };
};

module Button = {
  let noop = () => ();

  module Styles = {
    let box = (~disabled, ~isSelected) =>
      Style.[
        position(`Relative),
        backgroundColor(disabled ? Colors.dimGrey : isSelected ? Colors.lightSkyBlue : Colors.dodgerBlue),
        justifyContent(`Center),
        alignItems(`Center),
        border(~width=1, ~color=Colors.white),
        paddingVertical(4),
        paddingHorizontal(8),
      ];

    let text = 
      Style.[
        fontSize(Constants.fontSize),
        fontFamily("Roboto-Regular.ttf"),
        color(Colors.white),
        textWrap(TextWrapping.NoWrap),
      ];
  }

  let component = React.component("Button");

  let createElement =
      (
        ~children as _,
        ~label,
        ~onClick=noop,
        ~disabled=false,
        ~isSelected=false,
        ~tabindex=?,
        ~onFocus=?,
        ~onBlur=?,
        (),
      ) =>
    /* children, */
    component(slots =>
      (
        slots,
        <Clickable
          onClick={disabled ? noop : onClick} ?onFocus ?onBlur ?tabindex>
          <View style=Styles.box(~disabled, ~isSelected)>
            <Text style=Styles.text text=label />
          </View>
        </Clickable>,
      )
    );
}

module Checkbox = {
  let component = React.component("Checkbox");

  module Styles = {
    let box = isChecked =>
      Style.[
        width(24),
        height(24),
        border(~width=1, ~color=Colors.gray),
        backgroundColor(isChecked ? Colors.dodgerBlue : Colors.transparentWhite),
        justifyContent(`Center),
        alignItems(`Center),
      ];

    let checkmark = isChecked =>
      Style.[
        color(isChecked ? Colors.white : Colors.black),
        fontSize(16),
        fontFamily("FontAwesome5FreeSolid.otf"),
      ];
  }

  let createElement = (~children as _, ~isChecked, ~onToggle, ()) =>
    component(hooks => (
      hooks,
      <Clickable onClick=onToggle>
        <View style=Styles.box(isChecked)>
          <Text text={isChecked ? {||} : ""} style=Styles.checkmark(isChecked) />
        </View>
      </Clickable>,
    ));
}

module Todo = {
  module Styles = {
    let box =
      Style.[
        backgroundColor(Colors.white),
        flexDirection(`Row),
        margin(4),
      ];

    let text = 
      Style.[
        color(Colors.black),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(16),
        margin(2),
      ];
  }

  type t = {
    id: int,
    task: string,
    isDone: bool,
  };

  let component = React.component("TodoMVC");

  let createElement = (~children as _, ~task, ~onToggle, ()) =>
    component(hooks => (
      hooks,
      <View style=Styles.box>
        <Checkbox isChecked={task.isDone} onToggle />
        <Text style=Styles.text text={task.task} />
      </View>
    ));
}

module TodoMVC = {
  module Styles = {
    let container =
      Style.[
        position(`Absolute),
        top(0),
        bottom(0),
        left(0),
        right(0),
        alignItems(`Stretch),
        justifyContent(`Center),
        flexDirection(`Column),
        backgroundColor(Colors.whiteSmoke),
      ];

    let filterButtonsContainer =
      Style.[
        flexDirection(`Row),
        alignItems(`Center),
        justifyContent(`Center),
      ];

    let addTodoContainer =
      Style.[
        flexDirection(`Row),
      ]

    let input =
      Style.[
        fontSize(16),
      ];

    let todoList =
      Style.[
        flexGrow(1),
        border(~width=1, ~color=Colors.black),
      ];
  }

  type state = {
    todos: list(Todo.t),
    filter: Filter.t,
    inputValue: string,
    nextId: int,
  };

  let initialState = {
    todos: Todo.[
      { id: 0, task: "Buy Milk", isDone: false },
      { id: 1, task: "Wag the Dog", isDone: true },
    ],
    filter: All,
    inputValue: "",
    nextId: 2
  }

  type action =
    | AddTodo
    | ChangeFilter(Filter.t)
    | UpdateInputTextValue(string)
    | ToggleTaskState(int);

  let reducer = (action: action, state: state) =>
    switch (action) {
    | AddTodo => {
        ...state,
        todos: [
          {id: state.nextId, task: state.inputValue, isDone: false},
          ...state.todos
        ],
        inputValue: "",
        nextId: state.nextId + 1,
      }
    | UpdateInputTextValue(text) => {...state, inputValue: text}
    | ToggleTaskState(id) =>
      let todos =
        List.map(
          (item: Todo.t) => item.id == id ? {...item, isDone: !item.isDone} : item,
          state.todos,
        );
      {...state, todos};
    | ChangeFilter(filter) => {...state, filter}
    };

  let component = React.component("TodoMVC");

  let createElement = (~children as _, ()) =>
    component(hooks => {
      let ({todos, inputValue, filter as currentFilter, _}, dispatch, hooks) =
        Hooks.reducer(~initialState, reducer, hooks);

      let filteredTodos =
        List.filter(
          task =>
            switch (filter) {
            | All => true
            | Completed => task.Todo.isDone
            | NotCompleted => !task.Todo.isDone
            },
          todos,
        );

      let filterButtonsView = {
        let onSelect = filter => dispatch(ChangeFilter(filter));

        let button = filter =>
          <Button
            label={Filter.show(filter)}
            isSelected={currentFilter == filter}
            onClick={() => onSelect(filter)}
          />;

        <View style=Styles.filterButtonsContainer>
          {button(All)}
          {button(Completed)}
          {button(NotCompleted)}
        </View>
      };

      let addTodoView = {
        let onInput = value => dispatch(UpdateInputTextValue(value));
        let onButtonClick = () => dispatch(AddTodo);

        <View style=Styles.addTodoContainer>
          <Input
            style=Styles.input
            placeholder="Add your Todo here"
            value=inputValue
            onChange={({value, _}) => onInput(value)}
          />
          <Button label="+" disabled={inputValue == ""} onClick=onButtonClick />
        </View>
      };

      let todoListView = {
        let onToggle = (task: Todo.t) => () => dispatch(ToggleTaskState(task.id));

        <ScrollView style=Styles.todoList>
          <View>
            ...{
              filteredTodos
              |> List.map(task => <Todo task onToggle=onToggle(task)/>)
            }
          </View>
        </ScrollView>
      };

      (
        hooks,
        <View style=Styles.container>
          {filterButtonsView}
          {addTodoView}
          {todoListView}
        </View>,
      );
    });
};

let render = () => <TodoMVC />;
