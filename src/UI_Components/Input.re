open Revery_UI;
open Revery_Core;

type direction =
  | Static
  | Forward
  | Backward;

type state = {
  inputString: string,
  isFocused: bool,
  cursorPosition: int,
  highlightedText: string,
  highlightStart: option(int),
  highlightDirection: direction,
};

type textUpdate = {
  newString: string,
  cursorPosition: int,
};

type textHighlight = {
  direction,
  start: option(int),
  text: string,
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
  | SetFocus(bool)
  | ClearHighlight
  | HighlightText(textHighlight)
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

let getSafeCursorPosition = (str, cursorPosition, change) => {
  let nextPosition = cursorPosition + change;
  let currentLength = String.length(str);
  nextPosition > currentLength ?
    currentLength : nextPosition < 0 ? 0 : nextPosition;
};

/**
   getHighlightedText

   Return the section of a string which is being highlighted based on a start postion and the current posiition
 */
let getHighlightedText =
    (
      ~highlightStart,
      ~originalStart,
      ~cursorPosition,
      ~inputString,
      ~direction,
    ) => {
  let cursorPos = getSafeCursorPosition(inputString, cursorPosition, 0);
  switch (highlightStart, originalStart, direction) {
  | (Some(start), _, Backward) =>
    String.sub(inputString, cursorPos, start - cursorPos)
  | (Some(start), Some(original), Forward) =>
    print_endline("Start ---" ++ string_of_int(start));
    print_endline("cursorPos ---" ++ string_of_int(cursorPos));
    print_endline("Original ----" ++ string_of_int(original));
    String.sub(inputString, start, cursorPos - start);
  | (Some(start), None, Forward) => String.sub(inputString, start, 1)
  | (None, _, Forward) => String.sub(inputString, cursorPos, 1)
  | (None, _, Backward) =>
    let str = String.sub(inputString, cursorPos, 1);
    str;
  | (_, _, Static) => ""
  };
};

/**
   We check if there is an existing start position for the highlighting
   if there is we do not update, if there isn't OR the direction changed
   then we do.
 */
let updateHighlightStart = (prevStart, newStartPosition, directionChanged) =>
  switch (prevStart, directionChanged) {
  | (Some(previous), false) => Some(previous)
  | (Some(_), true) => newStartPosition
  | (None, _) => newStartPosition
  };

let reducer = (action, state) =>
  switch (action) {
  | SetFocus(isFocused) => {...state, isFocused}
  | ClearHighlight => {
      ...state,
      highlightDirection: Static,
      highlightedText: "",
      highlightStart: None,
    }
  | CursorPosition(pos) => {
      ...state,
      cursorPosition:
        getSafeCursorPosition(state.inputString, state.cursorPosition, pos),
    }
  | HighlightText({text, start, direction}) => {
      ...state,
      highlightDirection: direction,
      highlightStart: start,
      highlightedText: text,
    }
  | UpdateText({newString, cursorPosition}) =>
    state.isFocused ?
      {...state, cursorPosition, isFocused: true, inputString: newString} :
      state
  | Backspace({newString, cursorPosition}) =>
    state.isFocused ?
      {
        ...state,
        highlightStart: None,
        inputString: newString,
        cursorPosition,
      } :
      state
  };

let handleKeyPress =
    (
      ~state,
      ~dispatch,
      ~onChange: changeEvent => unit,
      event: NodeEvents.keyPressEventParams,
    ) =>
  addCharacter(state.inputString, event.character, state.cursorPosition)
  |> (
    update => {
      dispatch(ClearHighlight);
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
    }
  );

let directionToCursorPos = dir =>
  switch (dir) {
  | Forward => 1
  | Backward => (-1)
  | Static => 0
  };

let handleHighlighting =
    (
      ~onKeyDown,
      ~dispatch,
      ~state: state,
      event: NodeEvents.keyEventParams,
      direction,
    ) =>
  directionToCursorPos(direction)
  |> (
    change => {
      let newStart =
        updateHighlightStart(
          state.highlightStart,
          Some(state.cursorPosition),
          state.highlightDirection != direction,
        );

      onKeyDown(event);
      dispatch(CursorPosition(change));
      getHighlightedText(
        ~highlightStart=newStart,
        ~originalStart=state.highlightStart,
        /* use anticipated position */
        ~cursorPosition=state.cursorPosition + change,
        ~inputString=state.inputString,
        ~direction,
      )
      |> (
        highlighted =>
          dispatch(
            HighlightText({
              /* start position should be the position before the change */
              start: newStart,
              text: highlighted,
              direction,
            }),
          )
      );
    }
  );

let getUnhighlightedSegments =
    (~highlightStart, ~highlightDirection, ~startStr, ~endStr, ~inputString) =>
  switch (highlightStart, highlightDirection) {
  | (None, _) => (startStr, endStr)
  | (_, Static) => (startStr, endStr)
  | (Some(strt), Backward) =>
    strt
    - String.length(inputString)
    |> (offset => (startStr, Str.string_before(endStr, offset)))
  | (Some(strt), Forward) => (
      Str.string_before(startStr, strt - 1),
      endStr,
    )
  };

let handleKeyDown =
    (
      ~onKeyDown,
      ~onChange,
      ~dispatch,
      ~state: state,
      event: NodeEvents.keyEventParams,
    ) => {
  let highlightHandler =
    handleHighlighting(~onKeyDown, ~dispatch, ~state, event);
  switch (event) {
  | {key: Key.KEY_LEFT, shiftKey: true, _} => highlightHandler(Backward)
  | {key: Key.KEY_RIGHT, shiftKey: true, _} => highlightHandler(Forward)
  | {key: Key.KEY_LEFT, _} =>
    onKeyDown(event);
    dispatch(ClearHighlight);
    dispatch(CursorPosition(-1));
  | {key: Key.KEY_RIGHT, _} =>
    onKeyDown(event);
    dispatch(ClearHighlight);
    dispatch(CursorPosition(1));
  | {key: Key.KEY_BACKSPACE, _} =>
    dispatch(CursorPosition(-1));
    /**
     TODO: Remove the highlighted section
   */
    dispatch(ClearHighlight);
    removeCharacter(state.inputString, state.cursorPosition)
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
    dispatch(ClearHighlight);
    Focus.loseFocus();

  | _ => onKeyDown(event)
  };
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
    let (state, dispatch, slots) =
      React.Hooks.reducer(
        ~initialState={
          highlightDirection: Static,
          highlightStart: None,
          highlightedText: "",
          inputString: valueParam,
          cursorPosition: String.length(valueParam),
          isFocused: false,
        },
        reducer,
        slots,
      );

    let {
      highlightDirection,
      highlightStart,
      highlightedText,
      isFocused,
      cursorPosition,
      inputString,
    } = state;

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

    let inputTextStyles = hasMargin =>
      Style.[
        color(hasPlaceholder ? placeholderColor : inputColor),
        fontFamily(inputFontFamily),
        fontSize(inputFontSize),
        alignItems(`Center),
        justifyContent(`FlexStart),
        marginLeft(hasPlaceholder || hasMargin ? 0 : inputTextMargin),
      ];

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
          opacity(isFocused ? animatedOpacity : 0.0),
          backgroundColor(cursorColor),
        ]
      />;

    let makeTextComponent = (content, ~isEnd=false, ()) =>
      <Text text=content style={inputTextStyles(isEnd)} />;

    let (startStr, endStr) = getStringParts(cursorPosition, inputString);

    let (unselectedStart, unselectedEnd) =
      getUnhighlightedSegments(
        ~highlightStart,
        ~highlightDirection,
        ~startStr,
        ~endStr,
        ~inputString,
      );

    let placeholderText = makeTextComponent(placeholder, ());
    let startText = makeTextComponent(unselectedStart, ());
    let endText = makeTextComponent(unselectedEnd, ~isEnd=true, ());

    let highlighted =
      <Text
        text=highlightedText
        style=Style.[
          backgroundColor(Colors.grey),
          margin(0),
          ...inputTextStyles(true),
        ]
      />;

    print_endline("HighlightText ==============" ++ highlightedText);
    let keyDownHandler =
      handleKeyDown(~onKeyDown, ~dispatch, ~onChange, ~state);
    let keyPressHandler = handleKeyPress(~dispatch, ~onChange, ~state);

    (
      /*
         component
       */
      slots,
      <Clickable
        onFocus={() => dispatch(SetFocus(true))}
        onBlur={() => dispatch(SetFocus(false))}
        componentRef={autofocus ? Focus.focus : ignore}
        onKeyDown=keyDownHandler
        onKeyPress=keyPressHandler>
        <View style=viewStyles>
          ...{
               switch (hasPlaceholder, highlightDirection) {
               | (true, _) => [cursor, placeholderText]
               | (false, Backward) => [
                   startText,
                   cursor,
                   highlighted,
                   endText,
                 ]
               | (false, Forward) => [
                   startText,
                   highlighted,
                   cursor,
                   endText,
                 ]
               | (false, Static) => [startText, cursor, endText]
               }
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
