/* open Revery; */
/* open Revery.Core; */
/* open Revery.Core.Events; */
/* open Revery.Core.Window; */
/* open Revery.UI; */
/* open Revery.UI.Components; */

/* module Row = ( */
/*   val component((render, ~children, ()) => */
/*         render( */
/*           () => { */
/*             let style = */
/*               Style.make( */
/*                 ~flexDirection=LayoutTypes.Row, */
/*                 ~alignItems=LayoutTypes.AlignStretch, */
/*                 ~justifyContent=LayoutTypes.JustifyCenter, */
/*                 ~flexGrow=1, */
/*                 (), */
/*               ); */
/*             <view style> ...children </view>; */
/*           }, */
/*           ~children, */
/*         ) */
/*       ) */
/* ); */

/* module Column = ( */
/*   val component((render, ~children, ()) => */
/*         render( */
/*           () => { */
/*             let style = */
/*               Style.make( */
/*                 ~flexDirection=LayoutTypes.Column, */
/*                 ~alignItems=LayoutTypes.AlignStretch, */
/*                 ~justifyContent=LayoutTypes.JustifyCenter, */
/*                 ~backgroundColor=Colors.darkGrey, */
/*                 ~flexGrow=1, */
/*                 (), */
/*               ); */
/*             <view style> ...children </view>; */
/*           }, */
/*           ~children, */
/*         ) */
/*       ) */
/* ); */

/* module Button = ( */
/*   val component((render, ~fontFamily="Roboto-Regular.ttf", ~contents: string, ~onClick, ~children, ()) => */
/*         render( */
/*           () => { */
/*             let clickableStyle = */
/*               Style.make( */
/*                 ~position=LayoutTypes.Relative, */
/*                 ~backgroundColor=Colors.lightGrey, */
/*                 ~justifyContent=LayoutTypes.JustifyCenter, */
/*                 ~alignItems=LayoutTypes.AlignCenter, */
/*                 ~flexGrow=1, */
/*                 /1* Min width *1/ */
/*                 ~width=150, */
/*                 ~margin=10, */
/*                 (), */
/*               ); */
/*             let viewStyle = */
/*               Style.make( */
/*                 ~position=LayoutTypes.Relative, */
/*                 ~justifyContent=LayoutTypes.JustifyCenter, */
/*                 ~alignItems=LayoutTypes.AlignCenter, */
/*                 (), */
/*               ); */
/*             let textStyle = */
/*               Style.make( */
/*                 ~color=Colors.black, */
/*                 ~fontFamily, */
/*                 ~fontSize=32, */
/*                 (), */
/*               ); */

/*             <Clickable style=clickableStyle onClick> */
/*               <view style=viewStyle> */
/*                 <text style=textStyle> contents </text> */
/*               </view> */
/*             </Clickable>; */
/*           }, */
/*           ~children, */
/*         ) */
/*       ) */
/* ); */

/* module Display = ( */
/*   val component((render, ~display: string, ~curNum: string, ~children, ()) => */
/*         render( */
/*           () => { */
/*             let viewStyle = */
/*               Style.make( */
/*                 ~backgroundColor=Colors.white, */
/*                 ~height=120, */
/*                 ~flexDirection=LayoutTypes.Column, */
/*                 ~alignItems=LayoutTypes.AlignStretch, */
/*                 ~justifyContent=LayoutTypes.JustifyFlexStart, */
/*                 ~flexGrow=2, */
/*                 (), */
/*               ); */
/*             let displayStyle = */
/*               Style.make( */
/*                 ~color=Colors.black, */
/*                 ~fontFamily="Roboto-Regular.ttf", */
/*                 ~fontSize=20, */
/*                 ~margin=15, */
/*                 (), */
/*               ); */
/*             let numStyle = */
/*               Style.make( */
/*                 ~color=Colors.black, */
/*                 ~fontFamily="Roboto-Regular.ttf", */
/*                 ~fontSize=32, */
/*                 ~margin=15, */
/*                 (), */
/*               ); */

/*             <view style=viewStyle> */
/*               <text style=displayStyle> display </text> */
/*               <text style=numStyle> curNum </text> */
/*             </view>; */
/*           }, */
/*           ~children, */
/*         ) */
/*       ) */
/* ); */

/* type operator = [ | `Nop | `Add | `Sub | `Mul | `Div]; */

/* let showFloat = float => { */
/*   let string = string_of_float(float); */
/*   if (String.length(string) > 1 && string.[String.length(string) - 1] == '.') { */
/*     String.sub(string, 0, String.length(string) - 1); */
/*   } else { */
/*     string; */
/*   }; */
/* }; */
/* type state = { */
/*   operator, /1* Current operator being applied *1/ */
/*   result: float, /1* The actual numerical result *1/ */
/*   display: string, /1* The equation displayed *1/ */
/*   number: string /1* Current number being typed *1/ */
/* }; */

/* type action = */
/*   | BackspaceKeyPressed */
/*   | ClearKeyPressed(bool) /1* true = AC pressed *1/ */
/*   | DotKeyPressed */
/*   | NumberKeyPressed(string) */
/*   | OperationKeyPressed(operator) */
/*   | PlusMinusKeyPressed */
/*   | ResultKeyPressed; */

/* let eval = (state, newOp) => { */
/*   /1* Figure out what the string for the next operation will be *1/ */
/*   let newOpString = */
/*     switch (newOp) { */
/*     | `Nop => "" */
/*     | `Add => "+" */
/*     | `Sub => "-" */
/*     | `Mul => "×" */
/*     | `Div => "÷" */
/*     }; */
/*   /1* Split the current display on ! and get everything after (to clear errors) *1/ */
/*   let partitionedDisplay = String.split_on_char('!', state.display); */
/*   let display = */
/*     List.nth(partitionedDisplay, List.length(partitionedDisplay) - 1); */
/*   let (newDisplay, newResult) = */
/*     switch (state.operator) { */
/*     | #operator when state.number == "" => ( */
/*         "Error: Can't evaluate binary operator without input!", */
/*         state.result, */
/*       ) */
/*     | `Nop => (state.number ++ newOpString, float_of_string(state.number)) */
/*     | `Add => ( */
/*         display ++ state.number ++ newOpString, */
/*         state.result +. float_of_string(state.number), */
/*       ) */
/*     | `Sub => ( */
/*         display ++ state.number ++ newOpString, */
/*         state.result -. float_of_string(state.number), */
/*       ) */
/*     | `Mul => ( */
/*         display ++ state.number ++ newOpString, */
/*         state.result *. float_of_string(state.number), */
/*       ) */
/*     | `Div => */
/*       if (float_of_string(state.number) != 0.) { */
/*         ( */
/*           display ++ state.number ++ newOpString, */
/*           state.result /. float_of_string(state.number), */
/*         ); */
/*       } else { */
/*         ("Error: Divide by zero!", state.result); */
/*       } */
/*     }; */
/*   (newResult, newDisplay); */
/* }; */

/* let reducer = (state, action) => */
/*   switch (action) { */
/*   | BackspaceKeyPressed => */
/*     state.number == "" ? */
/*       state : */
/*       { */
/*         ...state, */
/*         number: String.sub(state.number, 0, String.length(state.number) - 1), */
/*       } */
/*   | ClearKeyPressed(ac) => */
/*     ac ? */
/*       {operator: `Nop, result: 0., display: "", number: ""} : */
/*       {...state, number: ""} */
/*   | DotKeyPressed => */
/*     String.contains(state.number, '.') ? */
/*       state : {...state, number: state.number ++ "."} */
/*   | NumberKeyPressed(n) => {...state, number: state.number ++ n} */
/*   | OperationKeyPressed(o) => */
/*     let (result, display) = eval(state, o); */
/*     {operator: o, result, display, number: ""}; */
/*   | PlusMinusKeyPressed => */
/*     if (state.number != "" && state.number.[0] == '-') { */
/*       { */
/*         ...state, */
/*         number: String.sub(state.number, 1, String.length(state.number) - 1), */
/*       }; */
/*     } else { */
/*       {...state, number: "-" ++ state.number}; */
/*     } */
/*   | ResultKeyPressed => */
/*     let (result, display) = eval(state, `Nop); */
/*     {operator: `Nop, result, display, number: showFloat(result)}; */
/*   }; */

/* module Calculator = ( */
/*   val component((render, ~window, ~children, ()) => */
/*         render( */
/*           () => { */
/*             let ({display, number, _}, dispatch) = */
/*               useReducer( */
/*                 reducer, */
/*                 {operator: `Nop, result: 0., display: "", number: ""}, */
/*               ); */

/*             let respondToKeys = e => switch(e.key) { */
/*               | Key.KEY_BACKSPACE => */
/*                 dispatch(BackspaceKeyPressed) */

/*               | Key.KEY_C when e.ctrlKey => dispatch(ClearKeyPressed(true)) */
/*               | Key.KEY_C => dispatch(ClearKeyPressed(false)) */

/*               /1* + key *1/ */
/*               | Key.KEY_EQUAL when e.shiftKey => dispatch(OperationKeyPressed(`Add)) */
/*               | Key.KEY_MINUS when e.ctrlKey => dispatch(PlusMinusKeyPressed) */
/*               | Key.KEY_MINUS => dispatch(OperationKeyPressed(`Sub)) */
/*               /1* * key *1/ */
/*               | Key.KEY_8 when e.shiftKey => dispatch(OperationKeyPressed(`Mul)) */
/*               | Key.KEY_SLASH => dispatch(OperationKeyPressed(`Div)) */
/*               | Key.KEY_PERIOD => dispatch(DotKeyPressed) */
/*               | Key.KEY_EQUAL => dispatch(ResultKeyPressed) */

/*               | Key.KEY_0 => dispatch(NumberKeyPressed("0")) */
/*               | Key.KEY_1 => dispatch(NumberKeyPressed("1")) */
/*               | Key.KEY_2 => dispatch(NumberKeyPressed("2")) */
/*               | Key.KEY_3 => dispatch(NumberKeyPressed("3")) */
/*               | Key.KEY_4 => dispatch(NumberKeyPressed("4")) */
/*               | Key.KEY_5 => dispatch(NumberKeyPressed("5")) */
/*               | Key.KEY_6 => dispatch(NumberKeyPressed("6")) */
/*               | Key.KEY_7 => dispatch(NumberKeyPressed("7")) */
/*               | Key.KEY_8 => dispatch(NumberKeyPressed("8")) */
/*               | Key.KEY_9 => dispatch(NumberKeyPressed("9")) */

/*               | _ => () */
/*             }; */
/*             /1* TODO: Pretty sure this isn't supposed to go in the render() function. */
/*                Seems to cause lag the more times we re-render, so I guess this is */
/*                subscribing a ton of times and never unsubscribing. *1/ */
/*             useEffect(() => { */
/*               let unsubscribe = Event.subscribe(window.onKeyDown, respondToKeys); */
/*               unsubscribe; */
/*             }); */

/*             <Column> */
/*               <Display display curNum=number /> */
/*               <Row> */
/*                 <Button */
/*                   contents="AC" */
/*                   onClick={_ => dispatch(ClearKeyPressed(true))} */
/*                 /> */
/*                 <Button */
/*                   contents="C" */
/*                   onClick={_ => dispatch(ClearKeyPressed(false))} */
/*                 /> */
/*                 <Button */
/*                   contents="±" */
/*                   onClick={_ => dispatch(PlusMinusKeyPressed)} */
/*                 /> */
/*                 /1* TODO: Switch to a font with a backspace character *1/ */
/*                 <Button */
/*                   fontFamily="FontAwesome5FreeSolid.otf" */
/*                   contents={||} */
/*                   onClick={_ => dispatch(BackspaceKeyPressed)} */
/*                 /> */
/*               </Row> */
/*               <Row> */
/*                 <Button */
/*                   contents="7" */
/*                   onClick={_ => dispatch(NumberKeyPressed("7"))} */
/*                 /> */
/*                 <Button */
/*                   contents="8" */
/*                   onClick={_ => dispatch(NumberKeyPressed("8"))} */
/*                 /> */
/*                 <Button */
/*                   contents="9" */
/*                   onClick={_ => dispatch(NumberKeyPressed("9"))} */
/*                 /> */
/*                 <Button */
/*                   contents="÷" */
/*                   onClick={_ => dispatch(OperationKeyPressed(`Div))} */
/*                 /> */
/*               </Row> */
/*               <Row> */
/*                 <Button */
/*                   contents="4" */
/*                   onClick={_ => dispatch(NumberKeyPressed("4"))} */
/*                 /> */
/*                 <Button */
/*                   contents="5" */
/*                   onClick={_ => dispatch(NumberKeyPressed("5"))} */
/*                 /> */
/*                 <Button */
/*                   contents="6" */
/*                   onClick={_ => dispatch(NumberKeyPressed("6"))} */
/*                 /> */
/*                 <Button */
/*                   contents="×" */
/*                   onClick={_ => dispatch(OperationKeyPressed(`Mul))} */
/*                 /> */
/*               </Row> */
/*               <Row> */
/*                 <Button */
/*                   contents="1" */
/*                   onClick={_ => dispatch(NumberKeyPressed("1"))} */
/*                 /> */
/*                 <Button */
/*                   contents="2" */
/*                   onClick={_ => dispatch(NumberKeyPressed("2"))} */
/*                 /> */
/*                 <Button */
/*                   contents="3" */
/*                   onClick={_ => dispatch(NumberKeyPressed("3"))} */
/*                 /> */
/*                 <Button */
/*                   contents="-" */
/*                   onClick={_ => dispatch(OperationKeyPressed(`Sub))} */
/*                 /> */
/*               </Row> */
/*               <Row> */
/*                 <Button contents="." onClick={_ => dispatch(DotKeyPressed)} /> */
/*                 <Button */
/*                   contents="0" */
/*                   onClick={_ => dispatch(NumberKeyPressed("0"))} */
/*                 /> */
/*                 <Button */
/*                   contents="=" */
/*                   onClick={_ => dispatch(ResultKeyPressed)} */
/*                 /> */
/*                 <Button */
/*                   contents="+" */
/*                   onClick={_ => dispatch(OperationKeyPressed(`Add))} */
/*                 /> */
/*               </Row> */
/*             </Column>; */
/*           }, */
/*           ~children, */
/*         ) */
/*       ) */
/* ); */

/* let init = app => { */
/*   let window = App.createWindow(app, "Revery Calculator"); */

/*   let render = () => { */
/*     <Calculator window />; */
/*   }; */

/*   UI.start(window, render); */
/* }; */

/* App.start(init); */
