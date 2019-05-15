open Revery_UI;
open Revery_Core;
open Revery_UI_Primitives;

module Hooks = Revery_UI_Hooks;

type state = {
  isFocused: bool,
  internalValue: string,
  cursorPosition: int,
  cursorTimer: Time.t,
};

type textUpdate = {
  newString: string,
  cursorPosition: int,
};

type cursorUpdate = {
  inputString: string,
  change: int,
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
  | CursorPosition(cursorUpdate)
  | CursorTimer
  | SetFocus(bool)
  | UpdateText(textUpdate)
  | ResetCursorTimer;

let getStringParts = (index, str) => {
  switch (index) {
  | 0 => ("", str)
  | _ =>
    let strBeginning =
      try (Str.string_before(str, index)) {
      | _ => str
      };
    let strEnd =
      try (Str.string_after(str, index)) {
      | _ => ""
      };
    (strBeginning, strEnd);
  };
};

let getSafeStringBounds = (str, cursorPosition, change) => {
  let nextPosition = cursorPosition + change;
  let currentLength = String.length(str);
  nextPosition > currentLength
    ? currentLength : nextPosition < 0 ? 0 : nextPosition;
};

let removeCharacterBefore = (word, cursorPosition) => {
  let (startStr, endStr) = getStringParts(cursorPosition, word);
  let nextPosition = getSafeStringBounds(startStr, cursorPosition, -1);
  let newString = Str.string_before(startStr, nextPosition) ++ endStr;
  {newString, cursorPosition: nextPosition};
};

let removeCharacterAfter = (word, cursorPosition) => {
  let (startStr, endStr) = getStringParts(cursorPosition, word);
  let newString =
    startStr
    ++ (
      switch (endStr) {
      | "" => ""
      | _ => Str.last_chars(endStr, String.length(endStr) - 1)
      }
    );
  {newString, cursorPosition};
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
  | CursorPosition({inputString, change}) => {
      ...state,
      cursorPosition:
        getSafeStringBounds(inputString, state.cursorPosition, change),
    }
  | CursorTimer => {
      ...state,
      cursorTimer:
        state.cursorTimer >= Time.Seconds(1.0)
          ? Time.Seconds(0.0)
          : Time.increment(state.cursorTimer, Time.Seconds(0.1)),
    }
  | UpdateText({newString, _}) =>
    state.isFocused
      ? {...state, isFocused: true, internalValue: newString} : state
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
      ~placeholder,
      ~cursorColor,
      ~placeholderColor,
      ~onChange,
      ~onKeyDown,
      ~value as valueAsProp,
      (),
    ) =>
  component(slots => {
    let (state, dispatch, slots) =
      Hooks.reducer(
        ~initialState={
          internalValue: "",
          cursorPosition:
            switch (valueAsProp) {
            | Some(v) => String.length(v)
            | None => 0
            },
          cursorTimer: Time.Seconds(0.0),
          isFocused: false,
        },
        reducer,
        slots,
      );

    let valueToDisplay =
      switch (valueAsProp) {
      | Some(v) => v
      | None => state.internalValue
      };

    let slots =
      Hooks.effect(
        OnMount,
        () => {
          let clear =
            Tick.interval(_ => dispatch(CursorTimer), Seconds(0.1));
          Some(clear);
        },
        slots,
      );

    let (inputValueRef, setInputValueRef, slots) =
      Hooks.ref(valueToDisplay, slots);

    let slots =
      Hooks.effect(
        If((!=), valueToDisplay),
        () => {
          let oldValueLength = String.length(inputValueRef);
          let newValueLength = String.length(valueToDisplay);
          switch (Pervasives.abs(oldValueLength - newValueLength)) {
          | lengthDiff when lengthDiff != 1 =>
            // Set cursor at the end
            dispatch(
              CursorPosition({
                inputString: valueToDisplay,
                change: String.length(valueToDisplay) - state.cursorPosition,
              }),
            )
          | _ =>
            let (oldStart, _oldEnd) =
              getStringParts(state.cursorPosition, inputValueRef);
            let (newStart, _newEnd) =
              getStringParts(state.cursorPosition, valueToDisplay);

            if (oldStart == newStart) {
              if (newValueLength > oldValueLength)
                {
                  // One character was added
                  dispatch(
                    CursorPosition({inputString: valueToDisplay, change: 1}),
                  );
                };
                // One characted was removed from the right side of the cursor (Delete key)
            } else {
              // One character was removed from the left side of the cursor (Backspace)
              dispatch(
                CursorPosition({inputString: valueToDisplay, change: (-1)}),
              );
            };
          };
          setInputValueRef(valueToDisplay);
          None;
        },
        slots,
      );

    let handleKeyPress = (event: NodeEvents.keyPressEventParams) => {
      let createChangeEvent = value => {
        value,
        key: Key.fromString(event.character),
        character: event.character,
        altKey: false,
        ctrlKey: false,
        shiftKey: false,
        superKey: false,
      };

      dispatch(ResetCursorTimer);

      switch (valueAsProp) {
      | Some(v) =>
        let {newString, _} =
          addCharacter(v, event.character, state.cursorPosition);
        onChange(createChangeEvent(newString));
      | None =>
        let {newString, cursorPosition} =
          addCharacter(
            state.internalValue,
            event.character,
            state.cursorPosition,
          );
        dispatch(UpdateText({newString, cursorPosition}));
        onChange(createChangeEvent(newString));
      };
    };

    let handleKeyDown = (event: NodeEvents.keyEventParams) => {
      let createChangeEvent = inputString => {
        value: inputString,
        character: Key.toString(event.key),
        key: event.key,
        altKey: event.altKey,
        ctrlKey: event.ctrlKey,
        shiftKey: event.shiftKey,
        superKey: event.superKey,
      };

      dispatch(ResetCursorTimer);

      switch (event.key) {
      | Key.KEY_LEFT =>
        onKeyDown(event);
        dispatch(
          CursorPosition({inputString: valueToDisplay, change: (-1)}),
        );
      | Key.KEY_RIGHT =>
        onKeyDown(event);
        dispatch(CursorPosition({inputString: valueToDisplay, change: 1}));
      | Key.KEY_DELETE =>
        // We should manage both cases
        removeCharacterAfter(valueToDisplay, state.cursorPosition)
        |> (
          update => {
            switch (valueAsProp) {
            | Some(_) => ()
            | None => dispatch(UpdateText(update))
            };
            onKeyDown(event);
            onChange(createChangeEvent(update.newString));
          }
        )

      | Key.KEY_BACKSPACE =>
        removeCharacterBefore(valueToDisplay, state.cursorPosition)
        |> (
          update => {
            switch (valueAsProp) {
            | Some(_) => ()
            | None => dispatch(UpdateText(update))
            };
            onKeyDown(event);
            onChange(createChangeEvent(update.newString));
          }
        )
      | Key.KEY_ESCAPE =>
        onKeyDown(event);
        Focus.loseFocus();
      | _ => onKeyDown(event)
      };
    };

    let hasPlaceholder = String.length(valueToDisplay) < 1;

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

    let cursorOpacity =
      state.isFocused
      |> (
        fun
        | true => state.cursorTimer <= Time.Seconds(0.5) ? 1.0 : 0.0
        | false => 0.0
      );

    let cursor = {
      let (startStr, _) =
        getStringParts(state.cursorPosition, valueToDisplay);
      let dimension =
        Revery_Draw.Text.measure(
          ~fontFamily=inputFontFamily,
          ~fontSize=inputFontSize,
          startStr,
        );
      <View
        style=Style.[
          width(2),
          marginTop((defaultHeight - dimension.height) / 2),
          height(inputFontSize),
          position(`Absolute),
          marginLeft(dimension.width + inputTextMargin + 1),
          opacity(cursorOpacity),
          backgroundColor(cursorColor),
        ]
      />;
    };

    let makeTextComponent = content =>
      <Text
        text=content
        style=Style.[
          color(hasPlaceholder ? placeholderColor : inputColor),
          fontFamily(inputFontFamily),
          fontSize(inputFontSize),
          alignItems(`Center),
          justifyContent(`FlexStart),
          marginLeft(inputTextMargin),
        ]
      />;

    let placeholderText = makeTextComponent(placeholder);
    let inputText = makeTextComponent(valueToDisplay);

    /*
       component
     */
    (
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
          cursor
          {hasPlaceholder ? placeholderText : inputText}
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
      ~placeholder="",
      ~onKeyDown=_ => (),
      ~onChange=_ => (),
      ~value=?,
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
