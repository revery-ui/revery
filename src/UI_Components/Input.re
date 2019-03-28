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

let removeCharacter = (word, offset) =>
  switch (String.length(word), offset) {
  | (0, n)
  | (_, n) when Pervasives.abs(n) == String.length(word) => word
  | (_, _) =>
    String.sub(word, 0, String.length(word) + offset - 1)
    ++ String.sub(
         word,
         String.length(word) + offset,
         Pervasives.abs(offset),
       )
  };

let addCharacter = (word, char, offset) =>
  String.sub(word, 0, String.length(word) + offset)
  ++ char
  ++ String.sub(word, String.length(word) + offset, Pervasives.abs(offset));

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

    let handleKeyDown = (event: NodeEvents.keyEventParams) =>
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
          onChange({
            character: Key.toString(event.key),
            key: event.key,
            altKey: event.altKey,
            ctrlKey: event.ctrlKey,
            shiftKey: event.shiftKey,
            superKey: event.superKey,
            value: removeCharacter(v, state.cursorOffset),
          })
        | None =>
          let newValue =
            removeCharacter(state.internalValue, state.cursorOffset);
          dispatch(SetValue(newValue));
          onChange({
            character: Key.toString(event.key),
            key: event.key,
            altKey: event.altKey,
            ctrlKey: event.ctrlKey,
            shiftKey: event.shiftKey,
            superKey: event.superKey,
            value: newValue,
          });
        }

      | Key.KEY_ESCAPE =>
        onKeyDown(event);
        Focus.loseFocus();
      | _ => onKeyDown(event)
      };

    let handleKeyPress = (event: NodeEvents.keyPressEventParams) => {
      let createChangeEvent = value => {
        key: Key.fromString(event.character),
        character: event.character,
        altKey: false,
        ctrlKey: false,
        shiftKey: false,
        superKey: false,
        value,
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

    let inputHeight = Selector.select(style, Height, 24);
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
    // marginLeft(6),

    /*
       TODO: this logic needs the equivalent of sizing an absolutely positioned
       element in css i.e. should work in the same way
       calculate the top padding needed to place the cursor centrally
     */
    let positionTop =
      inputHeight > inputFontSize
        ? (inputHeight - inputFontSize) / 2 : inputFontSize;

    let inputCursorStyles =
      Style.[
        marginLeft(hasPlaceholder ? inputTextMargin : 0),
        height(inputFontSize),
        width(2),
        opacity(state.isFocused ? animatedOpacity : 0.0),
        backgroundColor(cursorColor),
      ];
    // |> (
    //   initial =>
    //     hasPlaceholder
    //       ? Style.[
    //           position(`Absolute),
    //           // top(positionTop),
    //           left(5),
    //           ...initial,
    //         ]
    //       : initial
    // );

    /*
       component
     */
    (
      slots,
      <Clickable
        onFocus={() => dispatch(SetFocus(true))}
        onBlur={() => dispatch(SetFocus(false))}
        componentRef={state.autofocus ? Focus.focus : ignore}
        onKeyDown=handleKeyDown
        onKeyPress=handleKeyPress>
        {hasPlaceholder
           ? <View style=viewStyles>
               <View style=inputCursorStyles />
               <Text style=innerTextStyles text=content />
             </View>
           : (
             switch (state.cursorOffset) {
             | 0 =>
               <View style=viewStyles>
                 <Text style=innerTextStyles text=content />
                 <View style=inputCursorStyles />
               </View>
             | n when Pervasives.abs(n) == String.length(content) =>
               <View style=viewStyles>
                 <View style=inputCursorStyles />
                 <Text style=innerTextStyles text=content />
               </View>
             | offset =>
               <View style=viewStyles>
                 <Text
                   style=innerTextStyles
                   text={String.sub(
                     content,
                     0,
                     String.length(content) + offset,
                   )}
                 />
                 <View style=inputCursorStyles />
                 <Text
                   style=innerTextStyles
                   text={String.sub(
                     content,
                     String.length(content) + offset,
                     Pervasives.abs(offset),
                   )}
                 />
               </View>
             }
           )}
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