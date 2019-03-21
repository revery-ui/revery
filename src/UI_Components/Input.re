open Revery_UI;
open Revery_Core;

type state = {
  inputString: string,
  isFocused: bool,
  cursorPosition: int,
  highlightedText: string,
  highlightStart: option(int),
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

type textUpdate = {
  newString: string,
  cursorPosition: int,
};

type action =
  | CursorPosition(int)
  | SetFocus(bool)
  | HighlightText((option(int), string))
  | UpdateText(textUpdate)
  | Backspace(textUpdate);

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
  nextPosition > currentLength ?
    currentLength : nextPosition < 0 ? 0 : nextPosition;
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
  nextPosition > currentLength ?
    currentLength : nextPosition < 0 ? 0 : nextPosition;
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

type direction =
  | Forward
  | Backward;

let getHighlightedText =
    (highlightStart, cursorPosition, inputString, ~direction) =>
  try (
    switch (highlightStart, direction) {
    | (Some(start), Backward) =>
      String.sub(inputString, cursorPosition - 1, start - cursorPosition)
    | (Some(start), Forward) =>
      String.sub(inputString, start - cursorPosition, cursorPosition + 1)
    | (None, Forward) =>
      String.sub(inputString, cursorPosition, cursorPosition + 1)
    | (None, Backward) =>
      String.sub(inputString, cursorPosition, cursorPosition - 1)
    }
  ) {
  | Invalid_argument(s) =>
    print_endline("Failed because " ++ s);
    "";
  };

/**
   We check if there is an existing start position for the highlighting
   if there is we do not update, if there isn't then we do
 */
let updateHighlightStart = (prevStart: option(int), start: option(int)) =>
  switch (prevStart) {
  | Some(v) => Some(v)
  | None => start
  };

let reducer = (action, state) =>
  switch (action) {
  | SetFocus(isFocused) => {...state, isFocused}
  | CursorPosition(pos) => {
      ...state,
      cursorPosition:
        getSafeStringBounds(state.inputString, state.cursorPosition, pos),
    }
  | HighlightText((highlightStart, highlightedText)) => {
      ...state,
      highlightStart:
        updateHighlightStart(state.highlightStart, highlightStart),
      highlightedText,
    }
  | UpdateText({newString, cursorPosition}) =>
    state.isFocused ?
      {...state, cursorPosition, isFocused: true, inputString: newString} :
      state
  | Backspace({newString, cursorPosition}) =>
    state.isFocused ?
      {...state, inputString: newString, cursorPosition} : state
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
      {
        highlightStart,
        highlightedText,
        isFocused,
        cursorPosition,
        inputString,
      },
      dispatch,
      slots,
    ) =
      React.Hooks.reducer(
        ~initialState={
          highlightStart: None,
          highlightedText: "",
          inputString: valueParam,
          cursorPosition: String.length(valueParam),
          isFocused: false,
        },
        reducer,
        slots,
      );

    let handleKeyPress = (event: NodeEvents.keyPressEventParams) => {
      let update = addCharacter(inputString, event.character, cursorPosition);
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

    let handleKeyDown = (event: NodeEvents.keyEventParams) =>
      switch (event) {
      | {key: Key.KEY_LEFT, shiftKey: true, _} =>
        onKeyDown(event);
        dispatch(CursorPosition(-1));
        let highlighted =
          getHighlightedText(
            highlightStart,
            cursorPosition,
            inputString,
            ~direction=Backward,
          );
        dispatch(HighlightText((Some(cursorPosition), highlighted)));
      | {key: Key.KEY_RIGHT, shiftKey: true, _} =>
        onKeyDown(event);
        dispatch(CursorPosition(1));
        let highlighted =
          getHighlightedText(
            highlightStart,
            cursorPosition,
            inputString,
            ~direction=Forward,
          );
        dispatch(HighlightText((Some(cursorPosition), highlighted)));
      | {key: Key.KEY_LEFT, _} =>
        onKeyDown(event);
        dispatch(CursorPosition(-1));
      | {key: Key.KEY_RIGHT, _} =>
        onKeyDown(event);
        dispatch(CursorPosition(1));
      | {key: Key.KEY_BACKSPACE, _} =>
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
      | {key: Key.KEY_ESCAPE, _} =>
        onKeyDown(event);
        Focus.loseFocus();

      | _ => onKeyDown(event)
      };

    let (animatedOpacity, slots) =
      Hooks.animation(
        Animated.floatValue(0.),
        {
          toValue: 1.,
          duration: Seconds(0.5),
          delay: Seconds(0.5),
          repeat: true,
          easing: Animated.linear,
        },
        slots,
      );

    let hasPlaceholder = String.length(inputString) < 1;
    print_endline("highlightedText: " ++ highlightedText);

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

    let inputTextStyles = isEnd =>
      Style.[
        color(hasPlaceholder ? placeholderColor : inputColor),
        fontFamily(inputFontFamily),
        fontSize(inputFontSize),
        alignItems(`Center),
        justifyContent(`FlexStart),
        marginLeft(hasPlaceholder || isEnd ? 0 : inputTextMargin),
      ];

    /**
      We place these in a list so we change the order later to
      render the cursor before the text if placeholder is present
      otherwise to the cursor after
     */
    let cursor =
      <View
        style=Style.[
          width(1),
          marginLeft(hasPlaceholder ? inputTextMargin : 0),
          height(inputFontSize),
          opacity(isFocused ? animatedOpacity : 0.0),
          backgroundColor(cursorColor),
        ]
      />;

    let makeTextComponent = (content, ~isEnd) =>
      <Text text=content style={inputTextStyles(isEnd)} />;

    let (startStr, endStr) = getStringParts(cursorPosition, inputString);
    let placeholderText = makeTextComponent(placeholder, ~isEnd=false);
    let startText = makeTextComponent(startStr, ~isEnd=false);
    let endText = makeTextComponent(endStr, ~isEnd=true);
    print_endline("HighlightText ==============" ++ highlightedText);
    /* let highlightedText = makeTextComponent(highlightedText, ~isEnd=false); */

    (
      /*
         component
       */
      slots,
      <Clickable
        onFocus={() => dispatch(SetFocus(true))}
        onBlur={() => dispatch(SetFocus(false))}
        componentRef={autofocus ? Focus.focus : ignore}
        onKeyDown=handleKeyDown
        onKeyPress=handleKeyPress>
        <View style=viewStyles>
          ...{
               hasPlaceholder ?
                 [cursor, placeholderText] : [startText, cursor, endText]
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
