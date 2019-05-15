open Revery;
open Revery.UI;
open Revery.UI.Components;

module Row = {
  let component = React.component("Row");

  let createElement = (~children, ()) =>
    component(hooks => {
      let style =
        Style.[
          flexDirection(`Row),
          alignItems(`Stretch),
          justifyContent(`Center),
          flexGrow(1),
        ];
      (hooks, <View style> ...children </View>);
    });
};

module Column = {
  let component = React.component("Column");

  let createElement = (~children, ()) =>
    component(hooks => {
      let style =
        Style.[
          flexDirection(`Column),
          alignItems(`Stretch),
          justifyContent(`Center),
          backgroundColor(Colors.darkGrey),
          flexGrow(1),
        ];
      (hooks, <View style> ...children </View>);
    });
};

module Button = {
  let component = React.component("Button");

  let createElement =
      (
        ~fontFamily as family="Roboto-Regular.ttf",
        ~contents: string,
        ~onClick,
        ~children as _,
        (),
      ) =>
    component(hooks => {
      let clickableStyle =
        Style.[
          position(`Relative),
          backgroundColor(Colors.lightGrey),
          justifyContent(`Center),
          alignItems(`Center),
          flexGrow(1),
          /* Min width */
          width(125),
          margin(10),
        ];
      let viewStyle =
        Style.[
          position(`Relative),
          justifyContent(`Center),
          alignItems(`Center),
        ];
      let textStyle =
        Style.[color(Colors.black), fontFamily(family), fontSize(32)];

      (
        hooks,
        <Clickable style=clickableStyle onClick>
          <View style=viewStyle> <Text style=textStyle text=contents /> </View>
        </Clickable>,
      );
    });
};
module Display = {
  let component = React.component("Display");

  let createElement = (~display: string, ~curNum: string, ~children as _, ()) =>
    component(hooks => {
      let viewStyle =
        Style.[
          backgroundColor(Colors.white),
          height(120),
          flexDirection(`Column),
          alignItems(`Stretch),
          justifyContent(`FlexStart),
          flexGrow(2),
        ];
      let displayStyle =
        Style.[
          color(Colors.black),
          fontFamily("Roboto-Regular.ttf"),
          fontSize(20),
          margin(15),
        ];
      let numStyle =
        Style.[
          color(Colors.black),
          fontFamily("Roboto-Regular.ttf"),
          fontSize(32),
          margin(15),
        ];

      (
        hooks,
        <View style=viewStyle>
          <Text style=displayStyle text=display />
          <Text style=numStyle text=curNum />
        </View>,
      );
    });
};

type operator = [ | `Nop | `Add | `Sub | `Mul | `Div];

let showFloat = float => {
  let string = string_of_float(float);
  if (String.length(string) > 1 && string.[String.length(string) - 1] == '.') {
    String.sub(string, 0, String.length(string) - 1);
  } else {
    string;
  };
};
type state = {
  operator, /* Current operator being applied */
  result: float, /* The actual numerical result */
  display: string, /* The equation displayed */
  number: string /* Current number being typed */
};

type action =
  | BackspaceKeyPressed
  | ClearKeyPressed(bool) /* true = AC pressed */
  | DotKeyPressed
  | NumberKeyPressed(string)
  | OperationKeyPressed(operator)
  | PlusMinusKeyPressed
  | ResultKeyPressed;

let eval = (state, newOp) => {
  /* Figure out what the string for the next operation will be */
  let newOpString =
    switch (newOp) {
    | `Nop => ""
    | `Add => "+"
    | `Sub => "-"
    | `Mul => "×"
    | `Div => "÷"
    };
  /* Split the current display on ! and get everything after (to clear errors) */
  let partitionedDisplay = String.split_on_char('!', state.display);
  let display =
    List.nth(partitionedDisplay, List.length(partitionedDisplay) - 1);
  let (newDisplay, newResult) =
    switch (state.operator) {
    | #operator when state.number == "" => (
        "Error: Can't evaluate binary operator without input!",
        state.result,
      )
    | `Nop => (state.number ++ newOpString, float_of_string(state.number))
    | `Add => (
        display ++ state.number ++ newOpString,
        state.result +. float_of_string(state.number),
      )
    | `Sub => (
        display ++ state.number ++ newOpString,
        state.result -. float_of_string(state.number),
      )
    | `Mul => (
        display ++ state.number ++ newOpString,
        state.result *. float_of_string(state.number),
      )
    | `Div =>
      if (float_of_string(state.number) != 0.) {
        (
          display ++ state.number ++ newOpString,
          state.result /. float_of_string(state.number),
        );
      } else {
        ("Error: Divide by zero!", state.result);
      }
    };
  (newResult, newDisplay);
};

let reducer = (action, state) =>
  switch (action) {
  | BackspaceKeyPressed =>
    state.number == ""
      ? state
      : {
        ...state,
        number: String.sub(state.number, 0, String.length(state.number) - 1),
      }
  | ClearKeyPressed(ac) =>
    ac
      ? {operator: `Nop, result: 0., display: "", number: ""}
      : {...state, number: ""}
  | DotKeyPressed =>
    String.contains(state.number, '.')
      ? state : {...state, number: state.number ++ "."}
  | NumberKeyPressed(n) => {...state, number: state.number ++ n}
  | OperationKeyPressed(o) =>
    let (result, display) = eval(state, o);
    {operator: o, result, display, number: ""};
  | PlusMinusKeyPressed =>
    if (state.number != "" && state.number.[0] == '-') {
      {
        ...state,
        number: String.sub(state.number, 1, String.length(state.number) - 1),
      };
    } else {
      {...state, number: "-" ++ state.number};
    }
  | ResultKeyPressed =>
    let (result, display) = eval(state, `Nop);
    {operator: `Nop, result, display, number: showFloat(result)};
  };

module KeyboardInput = {
  type state = {
    ref: option(node),
    hasFocus: bool,
  };

  type action =
    | Focused(bool)
    | SetRef(node);

  let reducer = (action, state) =>
    switch (action) {
    | Focused(v) => {...state, hasFocus: v}
    | SetRef(v) => {...state, ref: Some(v)}
    };
  let component = React.component("KeyboardInput");

  let createElement = (~children as _, ~dispatch as parentDispatch, ()) =>
    component(hooks => {
      let (v, dispatch, hooks) =
        Hooks.reducer(
          ~initialState={ref: None, hasFocus: false},
          reducer,
          hooks,
        );

      let hooks =
        Hooks.effect(
          Always,
          () => {
            if (!v.hasFocus) {
              switch (v.ref) {
              | Some(v) => Focus.focus(v)
              | None => ()
              };
            };
            None;
          },
          hooks,
        );

      let onBlur = () => {
        dispatch(Focused(false));
      };

      let onFocus = () => {
        dispatch(Focused(true));
      };
      let respondToKeys = (e: NodeEvents.keyEventParams) =>
        switch (e.key) {
        | Key.KEY_BACKSPACE => parentDispatch(BackspaceKeyPressed)

        | Key.KEY_C when e.ctrlKey => parentDispatch(ClearKeyPressed(true))
        | Key.KEY_C => parentDispatch(ClearKeyPressed(false))

        /* + key */
        | Key.KEY_EQUAL when e.shiftKey =>
          parentDispatch(OperationKeyPressed(`Add))
        | Key.KEY_MINUS when e.ctrlKey => parentDispatch(PlusMinusKeyPressed)
        | Key.KEY_MINUS => parentDispatch(OperationKeyPressed(`Sub))
        /* * key */
        | Key.KEY_8 when e.shiftKey =>
          parentDispatch(OperationKeyPressed(`Mul))
        | Key.KEY_SLASH => parentDispatch(OperationKeyPressed(`Div))
        | Key.KEY_PERIOD => parentDispatch(DotKeyPressed)
        | Key.KEY_EQUAL => parentDispatch(ResultKeyPressed)

        | Key.KEY_0 => parentDispatch(NumberKeyPressed("0"))
        | Key.KEY_1 => parentDispatch(NumberKeyPressed("1"))
        | Key.KEY_2 => parentDispatch(NumberKeyPressed("2"))
        | Key.KEY_3 => parentDispatch(NumberKeyPressed("3"))
        | Key.KEY_4 => parentDispatch(NumberKeyPressed("4"))
        | Key.KEY_5 => parentDispatch(NumberKeyPressed("5"))
        | Key.KEY_6 => parentDispatch(NumberKeyPressed("6"))
        | Key.KEY_7 => parentDispatch(NumberKeyPressed("7"))
        | Key.KEY_8 => parentDispatch(NumberKeyPressed("8"))
        | Key.KEY_9 => parentDispatch(NumberKeyPressed("9"))

        | _ => ()
        };
      (
        hooks,
        <View
          ref={r => dispatch(SetRef(r))}
          onBlur
          onFocus
          style=Style.[position(`Absolute), width(1), height(1)]
          onKeyDown=respondToKeys
        />,
      );
    });
};

module Calculator = {
  let component = React.component("Calculator");

  let createElement = (~children as _, ()) =>
    component(hooks => {
      let ({display, number, _}, dispatch, hooks) =
        Hooks.reducer(
          ~initialState={operator: `Nop, result: 0., display: "", number: ""},
          reducer,
          hooks,
        );

      (
        hooks,
        <Column>
          <KeyboardInput dispatch />
          <Display display curNum=number />
          <Row>
            <Button
              contents="AC"
              onClick={_ => dispatch(ClearKeyPressed(true))}
            />
            <Button
              contents="C"
              onClick={_ => dispatch(ClearKeyPressed(false))}
            />
            <Button
              contents="±"
              onClick={_ => dispatch(PlusMinusKeyPressed)}
            />
            /* TODO: Switch to a font with a backspace character */
            <Button
              fontFamily="FontAwesome5FreeSolid.otf"
              contents={||}
              onClick={_ => dispatch(BackspaceKeyPressed)}
            />
          </Row>
          <Row>
            <Button
              contents="7"
              onClick={_ => dispatch(NumberKeyPressed("7"))}
            />
            <Button
              contents="8"
              onClick={_ => dispatch(NumberKeyPressed("8"))}
            />
            <Button
              contents="9"
              onClick={_ => dispatch(NumberKeyPressed("9"))}
            />
            <Button
              contents="÷"
              onClick={_ => dispatch(OperationKeyPressed(`Div))}
            />
          </Row>
          <Row>
            <Button
              contents="4"
              onClick={_ => dispatch(NumberKeyPressed("4"))}
            />
            <Button
              contents="5"
              onClick={_ => dispatch(NumberKeyPressed("5"))}
            />
            <Button
              contents="6"
              onClick={_ => dispatch(NumberKeyPressed("6"))}
            />
            <Button
              contents="×"
              onClick={_ => dispatch(OperationKeyPressed(`Mul))}
            />
          </Row>
          <Row>
            <Button
              contents="1"
              onClick={_ => dispatch(NumberKeyPressed("1"))}
            />
            <Button
              contents="2"
              onClick={_ => dispatch(NumberKeyPressed("2"))}
            />
            <Button
              contents="3"
              onClick={_ => dispatch(NumberKeyPressed("3"))}
            />
            <Button
              contents="-"
              onClick={_ => dispatch(OperationKeyPressed(`Sub))}
            />
          </Row>
          <Row>
            <Button contents="." onClick={_ => dispatch(DotKeyPressed)} />
            <Button
              contents="0"
              onClick={_ => dispatch(NumberKeyPressed("0"))}
            />
            <Button contents="=" onClick={_ => dispatch(ResultKeyPressed)} />
            <Button
              contents="+"
              onClick={_ => dispatch(OperationKeyPressed(`Add))}
            />
          </Row>
        </Column>,
      );
    });
};

let render = _ => <Calculator />;
