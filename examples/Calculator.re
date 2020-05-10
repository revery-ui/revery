open Revery;
open Revery.UI;
open Revery.UI.Components;

module Row = {
  let make = (~children, ()) => {
    let style =
      Style.[
        flexDirection(`Row),
        alignItems(`Stretch),
        justifyContent(`Center),
        flexGrow(1),
      ];
    <View style> ...children </View>;
  };
};

module Column = {
  let make = (~children, ()) => {
    let style =
      Style.[
        flexDirection(`Column),
        alignItems(`Stretch),
        justifyContent(`Center),
        backgroundColor(Colors.darkGrey),
        flexGrow(1),
      ];
    <View style> ...children </View>;
  };
};

module Button = {
  let make =
      (~fontFamily=Font.Family.default, ~contents: string, ~onClick, ()) => {
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
      Style.[color(Colors.black), textWrap(TextWrapping.NoWrap)];

    <Clickable style=clickableStyle onClick>
      <View style=viewStyle>
        <Text style=textStyle fontFamily fontSize=32. text=contents />
      </View>
    </Clickable>;
  };
};

module Display = {
  let make = (~display: string, ~curNum: string, ()) => {
    let viewStyle =
      Style.[
        backgroundColor(Colors.white),
        height(120),
        flexDirection(`Column),
        alignItems(`Stretch),
        justifyContent(`FlexStart),
        flexGrow(2),
      ];
    let displayStyle = Style.[color(Colors.black), margin(15)];
    let numStyle = Style.[color(Colors.black), margin(15)];

    <View style=viewStyle>
      <Text style=displayStyle fontSize=20. text=display />
      <Text style=numStyle fontSize=32. text=curNum />
    </View>;
  };
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

  let%component make = (~dispatch as parentDispatch, ()) => {
    let%hook (v, dispatch) =
      Hooks.reducer(~initialState={ref: None, hasFocus: false}, reducer);

    let%hook () =
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
      );

    let onBlur = () => {
      dispatch(Focused(false));
    };

    let onFocus = () => {
      dispatch(Focused(true));
    };

    let respondToKeys = (event: NodeEvents.keyEventParams) => {
      Key.Keycode.(
        switch (event.keycode) {
        | v when v == backspace => parentDispatch(BackspaceKeyPressed)
        | v when v == c && event.ctrlKey =>
          parentDispatch(ClearKeyPressed(true))
        | v when v == c => parentDispatch(ClearKeyPressed(false))

        /* + key */
        | v when v == Key.Keycode.equals && event.shiftKey =>
          parentDispatch(OperationKeyPressed(`Add))
        | v when v == Key.Keycode.equals && event.ctrlKey =>
          parentDispatch(PlusMinusKeyPressed)
        | v when v == Key.Keycode.minus =>
          parentDispatch(OperationKeyPressed(`Sub))
        /* * key */
        | v when v == digit8 && event.shiftKey =>
          parentDispatch(OperationKeyPressed(`Mul))
        | v when v == slash => parentDispatch(OperationKeyPressed(`Div))
        | v when v == period => parentDispatch(DotKeyPressed)
        | v when v == equals => parentDispatch(ResultKeyPressed)

        | v when v == digit0 => parentDispatch(NumberKeyPressed("0"))
        | v when v == digit1 => parentDispatch(NumberKeyPressed("1"))
        | v when v == digit2 => parentDispatch(NumberKeyPressed("2"))
        | v when v == digit3 => parentDispatch(NumberKeyPressed("3"))
        | v when v == digit4 => parentDispatch(NumberKeyPressed("4"))
        | v when v == digit5 => parentDispatch(NumberKeyPressed("5"))
        | v when v == digit6 => parentDispatch(NumberKeyPressed("6"))
        | v when v == digit7 => parentDispatch(NumberKeyPressed("7"))
        | v when v == digit8 => parentDispatch(NumberKeyPressed("8"))
        | v when v == digit9 => parentDispatch(NumberKeyPressed("9"))

        | _ => ()
        }
      );
    };

    <View
      ref={r => dispatch(SetRef(r))}
      onBlur
      onFocus
      style=Style.[position(`Absolute), width(1), height(1)]
      onKeyDown=respondToKeys
    />;
  };
};

module Calculator = {
  let%component make = () => {
    let%hook ({display, number, _}, dispatch) =
      Hooks.reducer(
        ~initialState={operator: `Nop, result: 0., display: "", number: ""},
        reducer,
      );

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
        <Button contents="±" onClick={_ => dispatch(PlusMinusKeyPressed)} />
        /* TODO: Switch to a font with a backspace character */
        <Button
          fontFamily={Font.Family.fromFile("FontAwesome5FreeSolid.otf")}
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
  };
};

let render = _ => <Calculator />;
