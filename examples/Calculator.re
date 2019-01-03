open Revery;
open Revery.Core;
open Revery.UI;
open Revery.UI.Components;

module Row = (
  val component((render, ~children, ()) =>
    render(() => {
      let style = Style.make(
        ~flexDirection=LayoutTypes.Row,
        ~alignItems=LayoutTypes.AlignStretch,
        ~justifyContent=LayoutTypes.JustifyCenter,
        ~flexGrow=1,
        ()
      );
      <view style>
        ...children
      </view>;
    }, ~children)
  )
);

module Column = (
  val component((render, ~children, ()) =>
    render(() => {
      let style = Style.make(
        ~flexDirection=LayoutTypes.Column,
        ~alignItems=LayoutTypes.AlignStretch,
        ~justifyContent=LayoutTypes.JustifyCenter,
        ~backgroundColor=Colors.darkGrey,
        ~flexGrow=1,
        ()
      );
      <view style>
        ...children
      </view>;
    }, ~children)
  )
);

module Button = (
  val component((render, ~contents: string, ~onClick, ~children, ()) =>
    render(() => {
      let viewStyle = Style.make(
        ~position=LayoutTypes.Relative,
        ~backgroundColor=Colors.lightGrey,
        ~justifyContent=LayoutTypes.JustifyCenter,
        ~alignItems=LayoutTypes.AlignCenter,
        /* TODO: Figure out relative sizing */
        ~width=150,
        ~height=120,
        ~flexGrow=1,
        ~margin=10,
        (),
      );
      let textStyle = Style.make(
        ~color=Colors.black,
        ~fontFamily="Roboto-Regular.ttf",
        ~fontSize=32,
        ()
      );

      <Clickable onClick>
        <view style=viewStyle>
          <text style=textStyle> contents </text>
        </view>
      </Clickable>;
    }, ~children)
  )
);

module Display = (
  val component((render, ~display: string, ~curNum: string, ~children, ()) =>
    render(() => {
      let viewStyle = Style.make(
        ~backgroundColor=Colors.white,
        ~height=120,
        ~flexDirection=LayoutTypes.Column,
        ~alignItems=LayoutTypes.AlignStretch,
        ~justifyContent=LayoutTypes.JustifyFlexEnd,
        ~flexGrow=2,
        ()
      );
      let displayStyle = Style.make(
        ~color=Colors.black,
        ~fontFamily="Roboto-Regular.ttf",
        ~fontSize=20,
        ~margin=15,
        ()
      );
      let numStyle = Style.make(
        ~color=Colors.black,
        ~fontFamily="Roboto-Regular.ttf",
        ~fontSize=32,
        ~margin=15,
        ()
      );

      <view style=viewStyle>
        <text style=displayStyle> display </text>
        <text style=numStyle> curNum </text>
      </view>;
    }, ~children)
  )
);

type operator =
  [ `Nop | `Add | `Sub | `Mul | `Div ];

let showFloat(float) {
  let string = string_of_float(float);
  if (String.length(string) > 1
      && string.[String.length(string) - 1] == '.') {
    String.sub(string, 0, String.length(string) - 1);
  } else {
    string;
  };
};

module Calculator = (
  val component((render, ~children, ()) =>
    render(() => {
      /* The equation displayed */
      let (display, setDisplay) = useState("");
      /* The actual numerical result */
      let (result, setResult) = useState(0.);
      /* Operator */
      let (op, setOp) = useState(`Nop);
      /* Current number being typed */
      let (curNum, setCurNum) = useState("");

      let eval(newOp) {
        /* Figure out what the string for the next operation will be */
        let newOpString = switch (newOp) {
        | `Nop => ""
        | `Add => "+"
        | `Sub => "-"
        | `Mul => "×"
        | `Div => "÷"
        };
        /* Split the current display on ! and get everything after (to clear errors) */
        let partitionedDisplay = String.split_on_char('!', display);
        let display = List.nth(partitionedDisplay, List.length(partitionedDisplay) - 1);
        let (newDisplay, newResult) = switch (op) {
        | #operator when curNum == "" =>
          ("Error: Can't evaluate binary operator without input!", result)
        | `Nop => (curNum ++ newOpString, float_of_string(curNum))
        | `Add => (display ++ curNum ++ newOpString, result +. float_of_string(curNum))
        | `Sub => (display ++ curNum ++ newOpString, result -. float_of_string(curNum))
        | `Mul => (display ++ curNum ++ newOpString, result *. float_of_string(curNum))
        | `Div =>
          if (float_of_string(curNum) != 0.) {
            (display ++ curNum ++ newOpString, result /. float_of_string(curNum));
          } else {
            ("Error: Divide by zero!", result);
          }
        };
        setResult(newResult);
        setDisplay(newDisplay);
        newResult;
      };

      let perform(newOp) {
        let _newResult = eval(newOp);
        setOp(newOp);
        setCurNum("");
      };

      let calculate() {
        let newResult = eval(`Nop);
        setOp(`Nop);
        setCurNum(showFloat(newResult));
      };

      let clear(~clearAcc) {
        if (clearAcc) {
          setOp(`Nop);
          setResult(0.);
          setDisplay("");
        };
        setCurNum("");
      };

      let negate() {
        if (curNum != "" && curNum.[0] == '-') {
          setCurNum(String.sub(curNum, 1, String.length(curNum) - 1));
        } else {
          setCurNum("-" ++ curNum);
        };
      };

      let backspace() {
        if (curNum != "") {
          setCurNum(String.sub(curNum, 0, String.length(curNum) - 1));
        };
      };

      <Column>
        <Display display curNum />
        <Row>
          <Button contents="AC" onClick=(_ => clear(~clearAcc=true)) />
          <Button contents="C" onClick=(_ => clear(~clearAcc=false)) />
          <Button contents="±" onClick=(_ => negate()) />
          /* TODO: Switch to a font with a backspace character */
          <Button contents="<=" onClick=(_ => backspace()) />
        </Row>
        <Row>
          <Button contents="7" onClick=(_ => setCurNum(curNum ++ "7")) />
          <Button contents="8" onClick=(_ => setCurNum(curNum ++ "8")) />
          <Button contents="9" onClick=(_ => setCurNum(curNum ++ "9")) />
          <Button contents="÷" onClick=(_ => perform(`Div)) />
        </Row>
        <Row>
          <Button contents="4" onClick=(_ => setCurNum(curNum ++ "4")) />
          <Button contents="5" onClick=(_ => setCurNum(curNum ++ "5")) />
          <Button contents="6" onClick=(_ => setCurNum(curNum ++ "6")) />
          <Button contents="×" onClick=(_ => perform(`Mul)) />
        </Row>
        <Row>
          <Button contents="1" onClick=(_ => setCurNum(curNum ++ "1")) />
          <Button contents="2" onClick=(_ => setCurNum(curNum ++ "2")) />
          <Button contents="3" onClick=(_ => setCurNum(curNum ++ "3")) />
          <Button contents="-" onClick=(_ => perform(`Sub)) />
        </Row>
        <Row>
          <Button contents="."
                  onClick=(_ => String.contains(curNum, '.')
                                  ? ()
                                  : setCurNum(curNum ++ ".")) />
          <Button contents="0" onClick=(_ => setCurNum(curNum ++ "0")) />
          <Button contents="=" onClick=(_ => calculate()) />
          <Button contents="+" onClick=(_ => perform(`Add)) />
        </Row>
      </Column>
    }, ~children)
  )
);

let init(app) {
  let window = App.createWindow(app, "Revery Calculator");

  let render() {
    <Calculator/>;
  };

  UI.start(window, render);
};

App.start(init);
