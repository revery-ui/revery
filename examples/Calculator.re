/* open Revery; */
open Revery.Core;
open Revery.Core.Events;
open Revery.Core.Window;
open Revery.UI;
open Revery.UI.Components;

module Row = {
  let component = React.component("Row");

  let make = children =>
    component((_slots: React.Hooks.empty) => {
      let style =
        Style.[
          flexDirection(`Row),
          alignItems(`Stretch),
          justifyContent(`Center),
          flexGrow(1),
        ];
      <View style> ...children </View>;
    });

  let createElement = (~children, ()) => React.element(make(children));
};

module Column = {
  let component = React.component("Column");

  let make = children =>
    component((_slots: React.Hooks.empty) => {
      let style =
        Style.[
          flexDirection(`Column),
          alignItems(`Stretch),
          justifyContent(`Center),
          backgroundColor(Colors.darkGrey),
          flexGrow(1),
        ];
      <View style> ...children </View>;
    });

  let createElement = (~children, ()) => React.element(make(children));
};

module Button = {
  let component = React.component("Button");

  let make =
      (
        ~fontFamily as family="Roboto-Regular.ttf",
        ~contents: string,
        ~onClick,
        (),
      ) =>
    component((_slots: React.Hooks.empty) => {
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

      <Clickable style=clickableStyle onClick>
        <View style=viewStyle> <Text style=textStyle text=contents /> </View>
      </Clickable>;
    });

  let createElement =
      (
        ~fontFamily="Roboto-Regular.ttf",
        ~contents,
        ~onClick,
        ~children as _,
        (),
      ) =>
    React.element(make(~fontFamily, ~contents, ~onClick, ()));
};
module Display = {
  let component = React.component("Display");

  let make = (~display: string, ~curNum: string, ()) =>
    component((_slots: React.Hooks.empty) => {
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

      <View style=viewStyle>
        <Text style=displayStyle text=display />
        <Text style=numStyle text=curNum />
      </View>;
    });

  let createElement = (~display: string, ~curNum: string, ~children as _, ()) =>
    React.element(make(~display, ~curNum, ()));
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

module Calculator = {
  let component = React.component("Calculator");

  let make = window =>
    component(slots => {
      let ({display, number, _}, dispatch, slots) =
        React.Hooks.reducer(
          ~initialState={operator: `Nop, result: 0., display: "", number: ""},
          reducer,
          slots,
        );

      let respondToKeys = e =>
        switch (e.key) {
        | Key.KEY_BACKSPACE => dispatch(BackspaceKeyPressed)

        | Key.KEY_C when e.ctrlKey => dispatch(ClearKeyPressed(true))
        | Key.KEY_C => dispatch(ClearKeyPressed(false))

        /* + key */
        | Key.KEY_EQUAL when e.shiftKey =>
          dispatch(OperationKeyPressed(`Add))
        | Key.KEY_MINUS when e.ctrlKey => dispatch(PlusMinusKeyPressed)
        | Key.KEY_MINUS => dispatch(OperationKeyPressed(`Sub))
        /* * key */
        | Key.KEY_8 when e.shiftKey => dispatch(OperationKeyPressed(`Mul))
        | Key.KEY_SLASH => dispatch(OperationKeyPressed(`Div))
        | Key.KEY_PERIOD => dispatch(DotKeyPressed)
        | Key.KEY_EQUAL => dispatch(ResultKeyPressed)

        | Key.KEY_0 => dispatch(NumberKeyPressed("0"))
        | Key.KEY_1 => dispatch(NumberKeyPressed("1"))
        | Key.KEY_2 => dispatch(NumberKeyPressed("2"))
        | Key.KEY_3 => dispatch(NumberKeyPressed("3"))
        | Key.KEY_4 => dispatch(NumberKeyPressed("4"))
        | Key.KEY_5 => dispatch(NumberKeyPressed("5"))
        | Key.KEY_6 => dispatch(NumberKeyPressed("6"))
        | Key.KEY_7 => dispatch(NumberKeyPressed("7"))
        | Key.KEY_8 => dispatch(NumberKeyPressed("8"))
        | Key.KEY_9 => dispatch(NumberKeyPressed("9"))

        | _ => ()
        };
      /* TODO: Pretty sure this isn't supposed to go in the render() function.
         Seems to cause lag the more times we re-render, so I guess this is
         subscribing a ton of times and never unsubscribing. */
      let _slots: React.Hooks.empty =
        React.Hooks.effect(
          OnMount,
          () => {
            let unsubscribe =
              Event.subscribe(window.onKeyDown, respondToKeys);
            Some(unsubscribe);
          },
          slots,
        );

      <Column>
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
      </Column>;
    });

  let createElement = (~window, ~children as _, ()) =>
    React.element(make(window));
};

let render = window => <Calculator window />;
