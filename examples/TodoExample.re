open Revery;
open Revery.UI;
open Revery.UI.Components;

module Constants = {
  let fontSize = 12;
}

module Theme = {
  let fontFamily = Style.fontFamily("Roboto-Regular.ttf");
  let fontSize = 16;

  let appBackground = Color.rgb(0.9, 0.93, 0.9);
  let textColor = Color.rgb(0.3, 0.32, 0.3);
  let dimmedTextColor = Color.rgb(0.8, 0.83, 0.8);
  let titleTextColor = Colors.darkSeaGreen;

  let panelBackground = Colors.white;
  let panelBorder = Style.border(~width=1, ~color=Color.rgb(0.85, 0.88, 0.85));

  let buttonBackground = Colors.mediumSeaGreen;
  let disabledButtonBackground = Colors.darkSeaGreen;
  let selectedButtonBackground = Colors.seaGreen;
  let buttonTextColor = Color.rgb(0.96, 1., 0.96);
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
  module Styles = {
    let box = (~isDisabled, ~isSelected) =>
      Style.[
        position(`Relative),
        justifyContent(`Center),
        alignItems(`Center),
        paddingVertical(6),
        paddingHorizontal(12),
        marginHorizontal(4),
        backgroundColor(
          switch (isDisabled, isSelected) {
          | (true, _) => Theme.disabledButtonBackground
          | (false, true) => Theme.selectedButtonBackground
          | (false, false) => Theme.buttonBackground
          }
        ),
      ];

    let text = 
      Style.[
        Theme.fontFamily,
        fontSize(Theme.fontSize),
        color(Theme.buttonTextColor),
        textWrap(TextWrapping.NoWrap),
      ];
  }

  let component = React.component("Button");

  let createElement =
      (
        ~children as _,
        ~label,
        ~onClick=?,
        ~isDisabled=false,
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
        <Clickable ?onClick ?onFocus ?onBlur ?tabindex>
          <View style=Styles.box(~isDisabled, ~isSelected)>
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
        width(int_of_float(float_of_int(Theme.fontSize) *. 1.5)),
        height(int_of_float(float_of_int(Theme.fontSize) *. 1.5)),
        justifyContent(`Center),
        alignItems(`Center),
        Theme.panelBorder,
        backgroundColor(isChecked ? Theme.buttonBackground : Colors.transparentWhite),
      ];

    let checkmark = isChecked =>
      Style.[
        color(isChecked ? Theme.panelBackground : Colors.transparentWhite),
        fontSize(Theme.fontSize),
        fontFamily("FontAwesome5FreeSolid.otf"),
        /* transform(Transform.[TranslateX(1.), TranslateY(2.)]), */
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

module AddTodo = {
  module Styles = {
    let container =
      Style.[
        flexDirection(`Row),
        backgroundColor(Theme.panelBackground),
        Theme.panelBorder,
        margin(2),
        alignItems(`Center),
      ];

    let toggleAll =
      Style.[
        color(Theme.textColor),
        fontSize(Theme.fontSize),
        fontFamily("FontAwesome5FreeSolid.otf"),
        transform(Transform.[TranslateY(2.)]),
        marginLeft(12),
      ];

    let input =
      Style.[
        fontSize(int_of_float(float_of_int(Theme.fontSize) *. 1.25)),
        border(~width=0, ~color=Colors.transparentWhite),
      ];
  }

  let component = React.component("TodoMVC");

  let createElement = (~children as _, ~text, ~onInput, ~onSubmit, ()) =>
    component(hooks => {
      let onKeyDown = (event: NodeEvents.keyEventParams) =>
        if (event.keycode == 13) {
          onSubmit();
        };
      
      (
        hooks,
        <View style=Styles.container>
          <Clickable onClick=((_) => ())>
            <Text text={||} style=Styles.toggleAll />
          </Clickable>
          <Input
            style=Styles.input
            placeholder="Add your Todo here"
            value=text
            onChange={({value, _}) => onInput(value)}
            onKeyDown
          />
        </View>
      )
    });
}

module Todo = {
  module Styles = {
    let box =
      Style.[
        flexDirection(`Row),
        margin(2),
        paddingVertical(4),
        paddingHorizontal(8),
        alignItems(`Center),
        backgroundColor(Theme.panelBackground),
        Theme.panelBorder,
      ];

    let text = isChecked =>
      Style.[
        margin(6),
        Theme.fontFamily,
        fontSize(Theme.fontSize),
        color(isChecked ? Theme.dimmedTextColor : Theme.textColor),
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
        <Text style=Styles.text(task.isDone) text={task.task} />
      </View>
    ));
}

module TodoMVC = {
  module Styles = {
    let appContainer =
      Style.[
        position(`Absolute),
        top(0),
        bottom(0),
        left(0),
        right(0),
        alignItems(`Stretch),
        justifyContent(`Center),
        flexDirection(`Column),
        backgroundColor(Theme.appBackground),
        paddingVertical(2),
        paddingHorizontal(6),
        overflow(`Hidden),
      ];

    let title =
      Style.[
        Theme.fontFamily,
        fontSize(Theme.fontSize * 4),
        color(Theme.titleTextColor),
        alignSelf(`Center),
        marginBottom(Theme.fontSize / 4),
        marginTop(Theme.fontSize * 2),
      ];

    let todoList =
      Style.[
        flexGrow(1),
      ];

    let filterButtonsContainer =
      Style.[
        flexDirection(`Row),
        alignItems(`Center),
        justifyContent(`Center),
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
        let onSubmit = () => dispatch(AddTodo);

        <AddTodo text=inputValue onInput onSubmit />
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
        <View style=Styles.appContainer>
          <Text text="todoMVC" style=Styles.title />
          {addTodoView}
          {todoListView}
          {filterButtonsView}
        </View>,
      );
    });
};

let render = () => <TodoMVC />;
