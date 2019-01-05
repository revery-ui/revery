open Revery.UI;

open Revery.UI.Components;
open Revery.Core;
open Revery.Core.Window;
open Revery.Core.Events;
open Revery.Core.Key;

type todo = {
  id: int,
  description: string,
};

type state = {
  currentInput: string,
  todos: list(todo),
};

type action =
  | UpdateInput(string)
  | AddTodo(todo)
  | RemoveTodo(int)
  | Backspace
  | CleanAll;

let reducer = (state, action) => {
  switch (action) {
  | UpdateInput(currentChar) => {
      ...state,
      currentInput: state.currentInput ++ currentChar,
    }
  | AddTodo(todo) => {currentInput: "", todos: [todo, ...state.todos]}
  | RemoveTodo(id) =>
    let currentTodo = List.filter(todo => todo.id !== id, state.todos);
    {currentInput: "", todos: currentTodo};
  | Backspace =>
    let length = String.length(state.currentInput);
    if (length > 0) {
      {
        ...state,
        currentInput: String.sub(state.currentInput, 0, length - 1),
      };
    } else {
      state;
    };
  | CleanAll => {currentInput: "", todos: []}
  };
};

let textStyle =
  Style.make(
    ~fontSize=20,
    ~fontFamily="Roboto-Regular.ttf",
    ~color=Colors.black,
    (),
  );

module TodoList = (
  val component((render, ~window, ~children, ()) =>
        render(
          () => {
            let ({currentInput, todos}, dispatch) =
              useReducer(reducer, {currentInput: "", todos: []});

            print_endline ("TODOS LENGTH: " ++ string_of_int(List.length(todos)));

            let response = keyEvent => {
              switch (keyEvent.key) {
              | KEY_ENTER =>
                dispatch(
                  AddTodo({
                    id: List.length(todos),
                    description: currentInput,
                  }),
                )
              | KEY_BACKSPACE => dispatch(Backspace)
              | _ => ()
              };
            };

            useEffect(() => {
              let unsubscribe = Event.subscribe(window.onKeyDown, response);
              unsubscribe;
            });

            useEffect(() => {
              let unsubscribe =
                Event.subscribe(window.onKeyPress, keyPress =>
                  dispatch(UpdateInput(keyPress.character))
                );
              unsubscribe;
            });

            let items =
              List.map(
                todo =>
                  <Clickable onClick={_ => dispatch(RemoveTodo(todo.id))}>
                    <view
                      style={Style.make(
                        ~height=30,
                        ~marginTop=5,
                        ~marginBottom=5,
                        ~marginLeft=20,
                        (),
                      )}>
                      <text style=textStyle> {todo.description} </text>
                    </view>
                  </Clickable>,
                todos,
              );
            <view style={Style.make(~flexGrow=1, ())}>
              <view
                style={Style.make(
                  ~height=50,
                  ~backgroundColor=Colors.green,
                  ~justifyContent=LayoutTypes.JustifyCenter,
                  ~alignItems=LayoutTypes.AlignCenter,
                  (),
                )}>
                <text style=textStyle> {currentInput ++ "|"} </text>
              </view>
              <view
                style={Style.make(
                  ~flexGrow=1,
                  ~backgroundColor=Colors.hotPink,
                  (),
                )}>
                ...items
              </view>
              <Button title="Clean All" onClick={_ => dispatch(CleanAll)} />
            </view>;
          },
          ~children,
        )
      )
);

let init = app => {
  let window = App.createWindow(app, "TodoList");

  let render = () => {
print_endline ("RENDERING");
    <TodoList window />;
  };

  Revery.UI.start(window, render);
  Window.setShouldRenderCallback(window, () => true);

};

App.start(init);
