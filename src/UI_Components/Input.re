open Revery_UI;
open Revery_Core;

type state = {
  inputString: string,
  isFocused: bool,
  lastKeyPress: option(Time.t),
  cursorPosition: int,
  cursorTimer: Time.t,
};

type textUpdate = {
  newString: string,
  cursorPosition: int,
};

type changeEvent = {
  value: string,
  character: string,
  key: Key.t,
  altKey: bool,
  ctrlKey: bool,
  shiftKey: bool,
  superKey: bool,
};

type action =
  | CursorPosition(int)
  | CursorTimer
  | SetFocus(bool)
  | UpdateLastKeyPress(Time.t)
  | UpdateText(textUpdate)
  | Backspace(textUpdate)
  | ResetCursorTimer;

let getStringParts = (index, str) =>
  switch (index) {
  | 0 => ("", str)
  | _ =>
    let strBeginning = Str.string_before(str, index);
    let strEnd = Str.string_after(str, index);
    (strBeginning, strEnd);
  };

let getSafeStringBounds = (str, cursorPosition, change) => {
  let nextPosition = cursorPosition + change;
  let currentLength = String.length(str);
  nextPosition > currentLength
    ? currentLength : nextPosition < 0 ? 0 : nextPosition;
};

let removeCharacter = (word, cursorPosition) => {
  let (startStr, endStr) = getStringParts(cursorPosition, word);
  let nextPosition = getSafeStringBounds(startStr, cursorPosition, -1);
  let newString = Str.string_before(startStr, nextPosition) ++ endStr;
  {newString, cursorPosition: nextPosition};
};

let addCharacter = (word, char, index) => {
  let (startStr, endStr) = getStringParts(index, word);
  {
    newString: startStr ++ char ++ endStr,
    cursorPosition: String.length(startStr) + 1,
  };
};

let reducer = (action, state) =>
  switch (action) {
  | SetFocus(isFocused) => {...state, isFocused}
  | CursorPosition(pos) => {
      ...state,
      cursorPosition:
        getSafeStringBounds(state.inputString, state.cursorPosition, pos),
    }
  | CursorTimer => {
      ...state,
      cursorTimer:
        state.cursorTimer >= Time.Seconds(1.0)
          ? Time.Seconds(0.0)
          : Time.increment(state.cursorTimer, Time.Seconds(0.1)),
    }
  | UpdateLastKeyPress(time) => {...state, lastKeyPress: Some(time)}
  | UpdateText({newString, cursorPosition}) =>
    state.isFocused
      ? {...state, cursorPosition, isFocused: true, inputString: newString}
      : state
  | Backspace({newString, cursorPosition}) =>
    state.isFocused
      ? {...state, inputString: newString, cursorPosition} : state
  | ResetCursorTimer => {...state, cursorTimer: Time.Seconds(0.0)}
  };

let defaultHeight = 50;
let defaultWidth = 200;
let inputTextMargin = 10;

let defaultStyles =
  Style.[
    color(Colors.black),
    width(defaultWidth),
    height(defaultHeight),
    border(
      /*
         The default border width should be 5% of the full input height
       */
      ~width=float_of_int(defaultHeight) *. 0.05 |> int_of_float,
      ~color=Colors.black,
    ),
    backgroundColor(Colors.transparentWhite),
  ];

let component = React.component("Input");

let make =
    (
      ~style,
      ~autofocus,
      ~value as valueParam,
      ~placeholder,
      ~cursorColor,
      ~placeholderColor,
      ~onChange,
      ~onKeyDown,
      (),
    ) =>
  component(slots => {
    let (
      {isFocused, cursorPosition, cursorTimer, inputString, lastKeyPress},
      dispatch,
      slots,
    ) =
      React.Hooks.reducer(
        ~initialState={
          inputString: valueParam,
          cursorPosition: String.length(valueParam),
          cursorTimer: Time.Seconds(0.0),
          isFocused: false,
          lastKeyPress: None,
        },
        reducer,
        slots,
      );

    let slots =
      React.Hooks.effect(
        OnMount,
        () => {
          let clear =
            Tick.interval(_ => dispatch(CursorTimer), Seconds(0.1));
          Some(clear);
        },
        slots,
      );

    let handleKeyPress = (event: NodeEvents.keyPressEventParams) => {
      let update = addCharacter(inputString, event.character, cursorPosition);

      dispatch(ResetCursorTimer);
      dispatch(UpdateLastKeyPress(Time.getTime()));
      dispatch(UpdateText(update));

      onChange({
        value: update.newString,
        key: Key.fromString(event.character),
        character: event.character,
        altKey: false,
        ctrlKey: false,
        shiftKey: false,
        superKey: false,
      });
    };

    let handleKeyDown = (event: NodeEvents.keyEventParams) => {
      dispatch(ResetCursorTimer);

      switch (event.key) {
      | Key.KEY_LEFT =>
        onKeyDown(event);
        dispatch(CursorPosition(-1));
      | Key.KEY_RIGHT =>
        onKeyDown(event);
        dispatch(CursorPosition(1));
      | Key.KEY_BACKSPACE =>
        dispatch(CursorPosition(-1));
        removeCharacter(inputString, cursorPosition)
        |> (
          update => {
            dispatch(Backspace(update));
            onKeyDown(event);
            onChange({
              value: update.newString,
              character: Key.toString(event.key),
              key: event.key,
              altKey: event.altKey,
              ctrlKey: event.ctrlKey,
              shiftKey: event.shiftKey,
              superKey: event.superKey,
            });
          }
        );
      | Key.KEY_ESCAPE =>
        onKeyDown(event);
        Focus.loseFocus();

      | _ => onKeyDown(event)
      };
    };

    let hasPlaceholder = String.length(inputString) < 1;

    /*
       computed styles
     */

    let allStyles =
      Style.(
        merge(
          ~source=[
            flexDirection(`Row),
            alignItems(`Center),
            justifyContent(`FlexStart),
            overflow(`Hidden),
            cursor(MouseCursors.text),
            ...defaultStyles,
          ],
          ~target=style,
        )
      );

    let viewStyles = Style.extractViewStyles(allStyles);
    let inputFontSize = Selector.select(style, FontSize, 18);
    let inputColor = Selector.select(style, Color, Colors.black);
    let inputFontFamily =
      Selector.select(style, FontFamily, "Roboto-Regular.ttf");

    let cursorOpacity = {
      let cursorToggleBreakpoint = Time.Seconds(0.5);

      let isTyping =
        lastKeyPress
        |> (
          fun
          | Some(timeAgo) =>
            timeAgo >= Time.increment(timeAgo, cursorToggleBreakpoint)
          | None => false
        );

      (isTyping, isFocused)
      |> (
        fun
        | (_, true) => cursorTimer <= cursorToggleBreakpoint ? 1.0 : 0.0
        | (_, false) => 0.0
      );
    };

    /**
      We place these in a list so we change the order later to
      render the cursor before the text if placeholder is present
      otherwise to the cursor after
     */
    let cursor =
      <View
        style=Style.[
          width(2),
          marginLeft(hasPlaceholder ? inputTextMargin : 0),
          height(inputFontSize),
          opacity(cursorOpacity),
          backgroundColor(cursorColor),
        ]
      />;

    let makeTextComponent = (content, ~isEnd) =>
      <Text
        text=content
        style=Style.[
          color(hasPlaceholder ? placeholderColor : inputColor),
          fontFamily(inputFontFamily),
          fontSize(inputFontSize),
          alignItems(`Center),
          justifyContent(`FlexStart),
          marginLeft(hasPlaceholder || isEnd ? 0 : inputTextMargin),
        ]
      />;

    let (startStr, endStr) = getStringParts(cursorPosition, inputString);
    let placeholderText = makeTextComponent(placeholder, ~isEnd=false);
    let startText = makeTextComponent(startStr, ~isEnd=false);
    let endText = makeTextComponent(endStr, ~isEnd=true);

    (
      /*
         component
       */
      slots,
      <Clickable
        onFocus={() => {
          dispatch(ResetCursorTimer);
          dispatch(SetFocus(true));
        }}
        onBlur={() => {
          dispatch(ResetCursorTimer);
          dispatch(SetFocus(false));
        }}
        componentRef={autofocus ? Focus.focus : ignore}
        onKeyDown=handleKeyDown
        onKeyPress=handleKeyPress>
        <View style=viewStyles>
          ...{
               hasPlaceholder
                 ? [cursor, placeholderText] : [startText, cursor, endText]
             }
        </View>
      </Clickable>,
    );
  });

let createElement =
    (
      ~children as _,
      ~style=defaultStyles,
      ~placeholderColor=Colors.grey,
      ~cursorColor=Colors.black,
      ~autofocus=false,
      ~value="",
      ~placeholder="",
      ~onKeyDown=_ => (),
      ~onChange=_ => (),
      (),
    ) =>
  make(
    ~value,
    ~style,
    ~placeholder,
    ~autofocus,
    ~cursorColor,
    ~placeholderColor,
    ~onKeyDown,
    ~onChange,
    (),
  );