open Revery_UI;
open Revery_Core;

type state = {
  isFocused: bool,
  internalValue: string,
  cursorOffset: int,
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
  | SetFocus(bool)
  | SetOffset(int)
  | SetValue(string);

let getTextBeforeCursor = (cursorOffset, text) =>
  String.sub(text, 0, String.length(text) + cursorOffset);
let getTextAfterCursor = (cursorOffset, text) =>
  String.sub(
    text,
    String.length(text) + cursorOffset,
    Pervasives.abs(cursorOffset),
  );

let removeCharacter = (word, offset) => {
  let wordLength = String.length(word);
  switch (wordLength, offset) {
  | (0, offset)
  | (_, offset) when Pervasives.abs(offset) == wordLength => word
  | (_, _) =>
    getTextBeforeCursor(offset - 1, word) ++ getTextAfterCursor(offset, word)
  };
};

let addCharacter = (word, char, offset) =>
  getTextBeforeCursor(offset, word)
  ++ char
  ++ getTextAfterCursor(offset, word);

let reducer = (action, state) =>
  /*
     TODO: Handle Cursor position changing via keyboard input e.g. arrow keys
     potentially draw the cursor Inside the text element and render the text around the cursor
   */
  switch (action) {
  | SetFocus(isFocused) => {...state, isFocused}
  | SetOffset(cursorOffset) => {...state, cursorOffset}
  | SetValue(internalValue) => {...state, internalValue}
  };

// let noop = (~value as _value) => ();
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

let createChangeEvent = (event, value: string): changeEvent => {
  character: Key.toString(event.key),
  key: event.key,
  altKey: event.altKey,
  ctrlKey: event.ctrlKey,
  shiftKey: event.shiftKey,
  superKey: event.superKey,
  value,
};

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
      React.Hooks.reducer(
        ~initialState={isFocused: false, internalValue: "", cursorOffset: 0},
        reducer,
        slots,
      );

    let valueToDisplay =
      switch (valueAsProp) {
      | Some(v) => v
      | None => state.internalValue
      };

    let handleKeyDown = (event: NodeEvents.keyEventParams) => {
      let createChangeEvent = value => {
        value,
        character: Key.toString(event.key),
        key: event.key,
        altKey: event.altKey,
        ctrlKey: event.ctrlKey,
        shiftKey: event.shiftKey,
        superKey: event.superKey,
      };
      switch (event.key) {
      | Key.KEY_LEFT =>
        onKeyDown(event);
        dispatch(
          SetOffset(
            Pervasives.max(
              - String.length(valueToDisplay),
              state.cursorOffset - 1,
            ),
          ),
        );
      | Key.KEY_RIGHT =>
        onKeyDown(event);
        dispatch(SetOffset(Pervasives.min(0, state.cursorOffset + 1)));
      | Key.KEY_BACKSPACE =>
        switch (valueAsProp) {
        | Some(v) =>
          onChange(
            createChangeEvent(removeCharacter(v, state.cursorOffset)),
          )
        | None =>
          let newValue =
            removeCharacter(state.internalValue, state.cursorOffset);
          dispatch(SetValue(newValue));
          onChange(createChangeEvent(newValue));
        }

      | Key.KEY_ESCAPE =>
        onKeyDown(event);
        Focus.loseFocus();
      | _ => onKeyDown(event)
      };
    };

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
      switch (valueAsProp) {
      | Some(v) =>
        onChange(
          createChangeEvent(
            addCharacter(v, event.character, state.cursorOffset),
          ),
        )
      | None =>
        let newValue =
          addCharacter(
            state.internalValue,
            event.character,
            state.cursorOffset,
          );
        dispatch(SetValue(newValue));
        onChange(createChangeEvent(newValue));
      };
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

    let hasPlaceholder = String.length(valueToDisplay) < 1;

    let content = hasPlaceholder ? placeholder : valueToDisplay;

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

    let innerTextStyles =
      Style.[
        color(hasPlaceholder ? placeholderColor : inputColor),
        fontFamily(inputFontFamily),
        fontSize(inputFontSize),
        alignItems(`Center),
        justifyContent(`FlexStart),
      ];

    let inputCursorStyles =
      Style.[
        height(inputFontSize),
        width(2),
        opacity(state.isFocused ? animatedOpacity : 0.0),
        backgroundColor(cursorColor),
      ];

    /*
       component
     */
    (
      slots,
      <Clickable
        onFocus={() => dispatch(SetFocus(true))}
        onBlur={() => dispatch(SetFocus(false))}
        componentRef={autofocus ? Focus.focus : ignore}
        onKeyDown=handleKeyDown
        onKeyPress=handleKeyPress>
        {hasPlaceholder
           ? <View style=viewStyles>
               <View
                 style=Style.(
                   merge(
                     ~source=inputCursorStyles,
                     ~target=Style.[marginLeft(inputTextMargin)],
                   )
                 )
               />
               <Text style=innerTextStyles text=content />
             </View>
           : <View style=viewStyles>
               ...{
                    switch (state.cursorOffset) {
                    | 0 => [
                        <Text
                          style=Style.(
                            merge(
                              ~source=innerTextStyles,
                              ~target=Style.[marginLeft(inputTextMargin)],
                            )
                          )
                          text=content
                        />,
                        <View style=inputCursorStyles />,
                      ]
                    | offset
                        when Pervasives.abs(offset) == String.length(content) => [
                        <View
                          style=Style.(
                            merge(
                              ~source=inputCursorStyles,
                              ~target=Style.[marginLeft(inputTextMargin)],
                            )
                          )
                        />,
                        <Text style=innerTextStyles text=content />,
                      ]
                    | offset => [
                        <Text
                          style=Style.(
                            merge(
                              ~source=innerTextStyles,
                              ~target=Style.[marginLeft(inputTextMargin)],
                            )
                          )
                          text={String.sub(
                            content,
                            0,
                            String.length(content) + offset,
                          )}
                        />,
                        <View style=inputCursorStyles />,
                        <Text
                          style=innerTextStyles
                          text={String.sub(
                            content,
                            String.length(content) + offset,
                            Pervasives.abs(offset),
                          )}
                        />,
                      ]
                    }
                  }
             </View>}
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