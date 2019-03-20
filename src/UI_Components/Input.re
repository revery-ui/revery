open Revery_UI;
open Revery_Core;

type state = {
  inputString: string,
  isFocused: bool,
  cursorPosition: int,
};

type textUpdate = {
  newString: string,
  cursorPosition: int,
};

type action =
  | CursorPosition(int)
  | SetFocus(bool)
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

let reducer = (action, state) =>
  /*
     TODO: Handle Cursor position changing via keyboard input e.g. arrow keys
     potentially draw the cursor Inside the text element and render the text around the cursor
   */
  switch (action) {
  | SetFocus(isFocused) => {...state, isFocused}
  | CursorPosition(pos) => {
      ...state,
      cursorPosition:
        getSafeStringBounds(state.inputString, state.cursorPosition, pos),
    }

  | UpdateText({newString, cursorPosition}) =>
    state.isFocused ?
      {cursorPosition, isFocused: true, inputString: newString} : state
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
      ~value as valueParam,
      ~placeholder,
      ~cursorColor,
      ~placeholderColor,
      ~onChange,
      (),
    ) =>
  component(slots => {
    let ({isFocused, cursorPosition, inputString}, dispatch, slots) =
      React.Hooks.reducer(
        ~initialState={
          inputString: valueParam,
          cursorPosition: String.length(valueParam),
          isFocused: false,
        },
        reducer,
        slots,
      );

    let handleKeyDown = (~dispatch, event: NodeEvents.keyEventParams) =>
      switch (event.key) {
      | Key.KEY_LEFT => dispatch(CursorPosition(-1))
      | Key.KEY_RIGHT => dispatch(CursorPosition(1))
      | Key.KEY_BACKSPACE =>
        dispatch(CursorPosition(-1));
        let update = removeCharacter(inputString, cursorPosition);
        dispatch(Backspace(update));
        onChange(~value=update.newString);
      | _ => ()
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

    /**
      We place these in a list so we change the order later to
      render the cursor before the text if placeholder is present
      otherwise to the cursor after
     */
    let cursor =
      <View
        style=Style.[
          width(2),
          marginLeft(hasPlaceholder ? inputTextMargin : 2),
          height(inputFontSize),
          opacity(isFocused ? animatedOpacity : 0.0),
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
          marginLeft(hasPlaceholder || isEnd ? 2 : inputTextMargin),
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
        onFocus={() => dispatch(SetFocus(true))}
        onBlur={() => dispatch(SetFocus(false))}
        onKeyDown={event => handleKeyDown(~dispatch, event)}
        onKeyPress={
          event => {
            let update =
              addCharacter(inputString, event.character, cursorPosition);
            dispatch(UpdateText(update));
            onChange(~value=update.newString);
          }
        }>
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
      ~value="",
      ~placeholder="",
      ~onChange=(~value as _) => (),
      (),
    ) =>
  make(
    ~value,
    ~style,
    ~placeholder,
    ~cursorColor,
    ~placeholderColor,
    ~onChange,
    (),
  );
