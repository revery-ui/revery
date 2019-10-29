open Revery;
open Revery.UI;
open Revery.UI.Components;

module Constants = {
  let fontSize = 12;
};

module Theme = {
  let fontFamily = Style.fontFamily("Roboto-Regular.ttf");
  let fontSize = 16;
  let rem = factor => int_of_float(float_of_int(fontSize) *. factor);

  let appBackground = Color.hex("#f4edfe");
  let textColor = Color.hex("#513B70");
  let dimmedTextColor = Color.hex("#DAC5F7");
  let titleTextColor = Color.hex("#EADDFC");

  let panelBackground = Color.hex("#F9F5FF");
  let panelBorderColor = Color.hex("#EADDFC");
  let panelBorder =
    Style.border(~width=1, ~color=panelBorderColor);

  let buttonColor = Color.hex("#9573C4");
  let hoveredButtonColor = Color.hex("#C9AEF0");

  let dangerColor = Color.hex("#f7c5c6");
};

module Filter = {
  type t =
    | All
    | Active
    | Completed;

  let show = (v: t) =>
    switch (v) {
    | All => "All"
    | Active => "Active"
    | Completed => "Completed"
    };
};

module Button = {
  module Styles = {
    let box = (~isSelected, ~isHovered) =>
      Style.[
        position(`Relative),
        justifyContent(`Center),
        alignItems(`Center),
        paddingVertical(Theme.rem(0.15)),
        paddingHorizontal(Theme.rem(0.5)),
        marginHorizontal(Theme.rem(0.2)),
        border(
          ~width=1,
          ~color=
            switch (isSelected, isHovered) {
            | (true, _) => Theme.buttonColor
            | (false, true) => Theme.hoveredButtonColor
            | (false, false) => Colors.transparentWhite
            },
        ),
        borderRadius(2.),
      ];

    let text =
      Style.[
        Theme.fontFamily,
        fontSize(Theme.rem(0.8)),
        color(Theme.buttonColor),
        textWrap(TextWrapping.NoWrap),
      ];
  };

  let component = React.component("Button");

  let createElement =
      (
        ~children as _,
        ~label,
        ~onClick=?,
        ~isSelected=false,
        ~tabindex=?,
        ~onFocus=?,
        ~onBlur=?,
        (),
      ) =>
    component(hooks => {
      let (isHovered, setHovered, hooks) = Hooks.state(false, hooks);

      (
        hooks,
        <Clickable ?onClick ?onFocus ?onBlur ?tabindex>
          <View
            style={Styles.box(~isSelected, ~isHovered)}
            onMouseOver={_ => setHovered(true)}
            onMouseOut={_ => setHovered(false)}>
            <Text style=Styles.text text=label />
          </View>
        </Clickable>,
      );
    });
};

module Checkbox = {
  let component = React.component("Checkbox");

  module Styles = {
    let box =
      Style.[
        width(Theme.rem(1.5)),
        height(Theme.rem(1.5)),
        justifyContent(`Center),
        alignItems(`Center),
        Theme.panelBorder,
      ];

    let checkmark = isChecked =>
      Style.[
        color(Theme.hoveredButtonColor),
        fontSize(Theme.fontSize),
        fontFamily("FontAwesome5FreeSolid.otf"),
        transform(Transform.[TranslateY(2.)]),
      ];
  };

  let createElement = (~children as _, ~isChecked, ~onToggle, ()) =>
    component(hooks =>
      (
        hooks,
        <Clickable onClick=onToggle>
          <View style=Styles.box>
            <Text
              text={isChecked ? {||} : ""}
              style={Styles.checkmark(isChecked)}
            />
          </View>
        </Clickable>,
      )
    );
};

module AddTodo = {
  module Styles = {
    let container =
      Style.[
        flexDirection(`Row),
        backgroundColor(Theme.panelBackground),
        Theme.panelBorder,
        margin(2),
        alignItems(`Center),
        overflow(`Hidden),
      ];

    let toggleAll = areAllCompleted =>
      Style.[
        color(areAllCompleted ? Theme.textColor : Theme.dimmedTextColor),
        fontSize(Theme.fontSize),
        fontFamily("FontAwesome5FreeSolid.otf"),
        transform(Transform.[TranslateY(2.)]),
        marginLeft(12),
      ];

    let input =
      Style.[
        fontSize(Theme.fontSize),
        border(~width=0, ~color=Colors.transparentWhite),
        width(4000),
      ]; // Not ideal, should be possible to use flexGrow(1) instead
  };

  let component = React.component("AddTodo");

  let createElement =
      (
        ~children as _,
        ~text,
        ~areAllCompleted,
        ~onInput,
        ~onSubmit,
        ~onToggleAll,
        (),
      ) =>
    component(hooks => {
      let onKeyDown = (event: NodeEvents.keyEventParams) =>
        if (event.keycode == 13) {
          onSubmit();
        };

      (
        hooks,
        <View style=Styles.container>
          <Clickable onClick=onToggleAll>
            <Text text={||} style={Styles.toggleAll(areAllCompleted)} />
          </Clickable>
          <Input
            style=Styles.input
            placeholder="Add your Todo here"
            value=text
            onChange={({value, _}) => onInput(value)}
            onKeyDown
          />
        </View>,
      );
    });
};

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
        flexGrow(1),
      ];

    let removeButton = isHovered =>
      Style.[
        color(isHovered ? Theme.dangerColor : Colors.transparentWhite),
        fontSize(Theme.fontSize),
        fontFamily("FontAwesome5FreeSolid.otf"),
        transform(Transform.[TranslateY(2.)]),
        marginRight(6),
      ];
  };

  type t = {
    id: int,
    task: string,
    isDone: bool,
  };

  let component = React.component("Todo");

  let createElement = (~children as _, ~task, ~onToggle, ~onRemove, ()) =>
    component(hooks => {
      let (isHovered, setHovered, hooks) = Hooks.state(false, hooks);

      (
        hooks,
        <View
          style=Styles.box
          onMouseOver={_ => setHovered(true)}
          onMouseOut={_ => setHovered(false)}>
          <Checkbox isChecked={task.isDone} onToggle />
          <Text style={Styles.text(task.isDone)} text={task.task} />
          <Clickable onClick=onRemove>
            <Text text={||} style={Styles.removeButton(isHovered)} />
          </Clickable>
        </View>,
      );
    });
};

module Footer = {
  module Styles = {
    let container =
      Style.[flexDirection(`Row), justifyContent(`SpaceBetween)];

    let filterButtonsContainer =
      Style.[
        flexGrow(1),
        width(0),
        flexDirection(`Row),
        alignItems(`Center),
        justifyContent(`Center),
        alignSelf(`Center),
        transform(Transform.[TranslateY(-2.)]),
      ];

    let leftFlexContainer = Style.[flexGrow(1), width(0)];

    let rightFlexContainer =
      Style.[
        flexGrow(1),
        width(0),
        flexDirection(`Row),
        justifyContent(`FlexEnd),
      ];

    let itemsLeft =
      Style.[
        Theme.fontFamily,
        fontSize(Theme.rem(0.85)),
        color(Theme.buttonColor),
        textWrap(TextWrapping.NoWrap),
      ];

    let clearCompleted = isHovered =>
      Style.[
        Theme.fontFamily,
        fontSize(Theme.rem(0.85)),
        color(isHovered ? Theme.hoveredButtonColor : Theme.buttonColor),
        textWrap(TextWrapping.NoWrap),
      ];
  };

  let component = React.component("Footer");

  let createElement =
      (
        ~children as _,
        ~activeCount,
        ~completedCount,
        ~currentFilter,
        ~onSelectFilter,
        ~onClearCompleted,
        (),
      ) =>
    component(hooks => {
      let itemsLeft = {
        let text =
          switch (activeCount) {
          | 1 => "1 item left"
          | n => Printf.sprintf("%i items left", n)
          };

        <Text text style=Styles.itemsLeft />;
      };

      let filterButtonsView = {
        let button = filter =>
          <Button
            label={Filter.show(filter)}
            isSelected={currentFilter == filter}
            onClick={() => onSelectFilter(filter)}
          />;

        <View style=Styles.filterButtonsContainer>
          {button(All)}
          {button(Active)}
          {button(Completed)}
        </View>;
      };

      let (hooks, clearCompleted) = {
        let (isHovered, setHovered, hooks) = Hooks.state(false, hooks);

        let text =
          switch (completedCount) {
          | 0 => ""
          | n => Printf.sprintf("Clear completed (%i)", n)
          };

        (
          hooks,
          <Clickable onClick=onClearCompleted>
            <View
              onMouseOver={_ => setHovered(true)}
              onMouseOut={_ => setHovered(false)}>
              <Text text style={Styles.clearCompleted(isHovered)} />
            </View>
          </Clickable>,
        );
      };

      (
        hooks,
        <View style=Styles.container>
          <View style=Styles.leftFlexContainer> itemsLeft </View>
          filterButtonsView
          <View style=Styles.rightFlexContainer> clearCompleted </View>
        </View>,
      );
    });
};

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
        fontSize(Theme.rem(4.)),
        color(Theme.titleTextColor),
        alignSelf(`Center),
        marginTop(Theme.rem(2.)),
        textWrap(TextWrapping.NoWrap),
      ];

    let todoList = Style.[flexGrow(1)];
  };

  type state = {
    todos: list(Todo.t),
    filter: Filter.t,
    inputValue: string,
    nextId: int,
  };

  let initialState = {
    todos:
      Todo.[
        {id: 0, task: "Buy Milk", isDone: false},
        {id: 1, task: "Wag the Dog", isDone: true},
      ],
    filter: All,
    inputValue: "",
    nextId: 2,
  };

  type action =
    | Add
    | SetFilter(Filter.t)
    | UpdateInput(string)
    | Toggle(int)
    | Remove(int)
    | ToggleAll
    | ClearCompleted;

  let reducer = (action: action, state: state) =>
    switch (action) {
    | Add => {
        ...state,
        todos: [
          {id: state.nextId, task: state.inputValue, isDone: false},
          ...state.todos,
        ],
        inputValue: "",
        nextId: state.nextId + 1,
      }

    | UpdateInput(text) => {...state, inputValue: text}

    | Toggle(id) =>
      let todos =
        List.map(
          (item: Todo.t) =>
            item.id == id ? {...item, isDone: !item.isDone} : item,
          state.todos,
        );
      {...state, todos};

    | Remove(id) =>
      let todos = List.filter((item: Todo.t) => item.id != id, state.todos);
      {...state, todos};

    | SetFilter(filter) => {...state, filter}

    | ToggleAll =>
      let areAllCompleted =
        List.for_all((item: Todo.t) => item.isDone, state.todos);
      let todos =
        List.map(
          (item: Todo.t) => {...item, isDone: !areAllCompleted},
          state.todos,
        );
      {...state, todos};

    | ClearCompleted =>
      let todos = List.filter((item: Todo.t) => !item.isDone, state.todos);
      {...state, todos};
    };

  let component = React.component("TodoMVC");

  let createElement = (~children as _, ()) =>
    component(hooks => {
      let ({todos, inputValue, filter as currentFilter, _}, dispatch, hooks) =
        Hooks.reducer(~initialState, reducer, hooks);

      let header = {
        <Text text="todoMVC" style=Styles.title />;
      };

      let addTodoView = {
        let onInput = value => dispatch(UpdateInput(value));
        let onSubmit = () => dispatch(Add);
        let onToggleAll = () => dispatch(ToggleAll);
        let areAllCompleted =
          List.for_all((item: Todo.t) => item.isDone, todos);

        <AddTodo
          text=inputValue
          areAllCompleted
          onInput
          onSubmit
          onToggleAll
        />;
      };

      let todoListView = {
        let onToggle = (id, ()) => dispatch(Toggle(id));
        let onRemove = (id, ()) => dispatch(Remove(id));
        let filteredTodos =
          List.filter(
            task =>
              switch (filter) {
              | All => true
              | Active => !task.Todo.isDone
              | Completed => task.Todo.isDone
              },
            todos,
          );

        <ScrollView style=Styles.todoList>
          <View>
            ...{List.map(
              (task: Todo.t) =>
                <Todo
                  task
                  onToggle={onToggle(task.id)}
                  onRemove={onRemove(task.id)}
                />,
              filteredTodos,
            )}
          </View>
        </ScrollView>;
      };

      let footer = {
        let onSelectFilter = filter => dispatch(SetFilter(filter));
        let onClearCompleted = () => dispatch(ClearCompleted);
        let activeCount =
          todos |> List.filter((item: Todo.t) => !item.isDone) |> List.length;
        let completedCount =
          todos |> List.filter((item: Todo.t) => item.isDone) |> List.length;

        <Footer
          activeCount
          completedCount
          currentFilter
          onSelectFilter
          onClearCompleted
        />;
      };

      (
        hooks,
        <View style=Styles.appContainer>
          header
          addTodoView
          todoListView
          footer
        </View>,
      );
    });
};

let render = () => <TodoMVC />;
