open Revery;
open Revery.UI;
open Revery.UI.Components;

module Constants = {
  let fontSize = 12.;
};

module Theme = {
  let fontSize = 16.;
  let rem = factor => fontSize *. factor;
  let remi = factor => rem(factor) |> int_of_float;

  let appBackground = Color.hex("#f4edfe");
  let textColor = Color.hex("#513B70");
  let dimmedTextColor = Color.hex("#DAC5F7");
  let titleTextColor = Color.hex("#EADDFC");

  let panelBackground = Color.hex("#F9F5FF");
  let panelBorderColor = Color.hex("#EADDFC");
  let panelBorder = Style.border(~width=1, ~color=panelBorderColor);

  let buttonColor = Color.hex("#9573C4");
  let hoveredButtonColor = Color.hex("#C9AEF0");

  let dangerColor = Color.hex("#f7c5c6");
};

module Filter = {
  type t =
    | All
    | Active
    | Completed;

  let toString = (v: t) =>
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
        paddingVertical(Theme.remi(0.15)),
        paddingHorizontal(Theme.remi(0.5)),
        marginHorizontal(Theme.remi(0.2)),
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
      Style.[color(Theme.buttonColor), textWrap(TextWrapping.NoWrap)];
  };

  let%component make =
                (
                  ~label,
                  ~onClick=?,
                  ~isSelected=false,
                  ~tabindex=?,
                  ~onFocus=?,
                  ~onBlur=?,
                  (),
                ) => {
    let%hook (isHovered, onMouseOver, onMouseOut) = Hooks.hover();

    <Clickable ?onClick ?onFocus ?onBlur ?tabindex>
      <View
        style={Styles.box(~isSelected, ~isHovered)} onMouseOver onMouseOut>
        <Text style=Styles.text text=label fontSize={Theme.rem(0.8)} />
      </View>
    </Clickable>;
  };
};

module Checkbox = {
  module Styles = {
    let box =
      Style.[
        width(Theme.remi(1.5)),
        height(Theme.remi(1.5)),
        justifyContent(`Center),
        alignItems(`Center),
        Theme.panelBorder,
      ];

    let checkmark =
      Style.[
        color(Theme.hoveredButtonColor),
        textWrap(TextWrapping.NoWrap),
        transform(Transform.[TranslateY(2.)]),
      ];
  };

  let make = (~isChecked, ~onToggle, ()) => {
    <Clickable onClick=onToggle>
      <View style=Styles.box>
        <Text
          text={isChecked ? {||} : ""}
          fontSize=Theme.fontSize
          fontFamily={Font.Family.fromFile("FontAwesome5FreeSolid.otf")}
          style=Styles.checkmark
        />
      </View>
    </Clickable>;
  };
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
        transform(Transform.[TranslateY(2.)]),
        marginLeft(12),
      ];

    let input =
      Style.[border(~width=0, ~color=Colors.transparentWhite), width(4000)]; // Not ideal, should be possible to use flexGrow(1) instead
  };

  let make = (~text, ~areAllCompleted, ~onInput, ~onSubmit, ~onToggleAll, ()) => {
    let onKeyDown = (event: NodeEvents.keyEventParams) =>
      if (event.keycode == 13) {
        onSubmit();
      };

    <View style=Styles.container>
      <Clickable onClick=onToggleAll>
        <Text
          text={||}
          fontSize=Theme.fontSize
          fontFamily={Font.Family.fromFile("FontAwesome5FreeSolid.otf")}
          style={Styles.toggleAll(areAllCompleted)}
        />
      </Clickable>
      <Input
        style=Styles.input
        fontSize=Theme.fontSize
        placeholder="Add your Todo here"
        value=text
        onChange={(value, _) => onInput(value)}
        onKeyDown
      />
    </View>;
  };
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
        color(isChecked ? Theme.dimmedTextColor : Theme.textColor),
        flexGrow(1),
      ];

    let removeButton = isHovered =>
      Style.[
        color(isHovered ? Theme.dangerColor : Colors.transparentWhite),
        transform(Transform.[TranslateY(2.)]),
        marginRight(6),
      ];
  };

  type t = {
    id: int,
    task: string,
    isDone: bool,
  };

  let%component make = (~task, ~onToggle, ~onRemove, ()) => {
    let%hook (isHovered, setHovered) = Hooks.state(false);

    <View
      style=Styles.box
      onMouseOver={_ => setHovered(_wasHovered => true)}
      onMouseOut={_ => setHovered(_wasHovered => false)}>
      <Checkbox isChecked={task.isDone} onToggle />
      <Text
        style={Styles.text(task.isDone)}
        fontSize=Theme.fontSize
        text={task.task}
      />
      <Clickable onClick=onRemove>
        <Text
          text={||}
          fontFamily={Font.Family.fromFile("FontAwesome5FreeSolid.otf")}
          fontSize=Theme.fontSize
          style={Styles.removeButton(isHovered)}
        />
      </Clickable>
    </View>;
  };
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
      Style.[color(Theme.buttonColor), textWrap(TextWrapping.NoWrap)];

    let clearCompleted = isHovered =>
      Style.[
        color(isHovered ? Theme.hoveredButtonColor : Theme.buttonColor),
        textWrap(TextWrapping.NoWrap),
      ];
  };

  let make =
      (
        ~activeCount,
        ~completedCount,
        ~currentFilter,
        ~onSelectFilter,
        ~onClearCompleted,
        (),
      ) => {
    let itemsLeft = {
      let text =
        switch (activeCount) {
        | 1 => "1 item left"
        | n => Printf.sprintf("%i items left", n)
        };

      <Text text fontSize={Theme.rem(0.85)} style=Styles.itemsLeft />;
    };

    let filterButtonsView = {
      let button = filter =>
        <Button
          label={Filter.toString(filter)}
          isSelected={currentFilter == filter}
          onClick={() => onSelectFilter(filter)}
        />;

      <View style=Styles.filterButtonsContainer>
        {button(All)}
        {button(Active)}
        {button(Completed)}
      </View>;
    };

    module ClearCompleted = {
      let%component make = (~onClearCompleted, ~completedCount, ()) => {
        let%hook (isHovered, setHovered) = Hooks.state(false);

        let text =
          switch (completedCount) {
          | 0 => ""
          | n => Printf.sprintf("Clear completed (%i)", n)
          };

        <Clickable onClick=onClearCompleted>
          <View
            onMouseOver={_ => setHovered(_wasHovered => true)}
            onMouseOut={_ => setHovered(_wasHovered => false)}>
            <Text
              text
              fontSize={Theme.rem(0.85)}
              style={Styles.clearCompleted(isHovered)}
            />
          </View>
        </Clickable>;
      };
    };

    <View style=Styles.container>
      <View style=Styles.leftFlexContainer> itemsLeft </View>
      filterButtonsView
      <View style=Styles.rightFlexContainer>
        <ClearCompleted onClearCompleted completedCount />
      </View>
    </View>;
  };
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
        color(Theme.titleTextColor),
        alignSelf(`Center),
        marginTop(Theme.remi(2.)),
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

  let%component make = () => {
    let%hook ({todos, inputValue, filter as currentFilter, _}, dispatch) =
      Hooks.reducer(~initialState, reducer);

    let header = {
      <Text text="todoMVC" fontSize={Theme.rem(4.)} style=Styles.title />;
    };

    let addTodoView = {
      let onInput = value => dispatch(UpdateInput(value));
      let onSubmit = () => dispatch(Add);
      let onToggleAll = () => dispatch(ToggleAll);
      let areAllCompleted =
        List.for_all((item: Todo.t) => item.isDone, todos);

      <AddTodo text=inputValue areAllCompleted onInput onSubmit onToggleAll />;
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
          {List.map(
             (task: Todo.t) =>
               <Todo
                 task
                 onToggle={onToggle(task.id)}
                 onRemove={onRemove(task.id)}
               />,
             filteredTodos,
           )
           |> React.listToElement}
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

    <View style=Styles.appContainer>
      header
      addTodoView
      todoListView
      footer
    </View>;
  };
};

let render = () => <TodoMVC />;
