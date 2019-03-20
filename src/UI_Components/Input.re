open Revery_UI;
open Revery_Core;

type state = {
  value: string,
  isFocused: bool,
};

type action =
  | SetFocus(bool)
  | UpdateText(string)
  | Backspace
  | ClearWord;

let removeCharacter = word =>
  String.length(word)
  |> (length => length > 0 ? String.sub(word, 0, length - 1) : word);

let addCharacter = (word, char) => word ++ char;

let reducer = (action, state) =>
  /*
     TODO: Handle Cursor position changing via keyboard input e.g. arrow keys
     potentially draw the cursor Inside the text element and render the text around the cursor
   */
  switch (action) {
  | SetFocus(isFocused) => {...state, isFocused}
  | UpdateText(t) =>
    state.isFocused ?
      {isFocused: true, value: addCharacter(state.value, t)} : state
  | Backspace =>
    state.isFocused ?
      {
        let length = String.length(state.value);
        length > 0 ? {...state, value: removeCharacter(state.value)} : state;
      } :
      state
  | ClearWord => {...state, value: ""}
  };

let handleKeyDown = (~dispatch, event: Events.keyEvent) =>
  switch (event.key) {
  | Key.KEY_BACKSPACE => dispatch(Backspace)
  | _ => ()
  };

let noop = (~value as _value) => ();

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
    let (state, dispatch, slots) =
      React.Hooks.reducer(
        ~initialState={value: valueParam, isFocused: false},
        reducer,
        slots,
      );

    let handleKeyDown = (~dispatch, event: NodeEvents.keyEventParams) =>
      switch (event.key) {
      | Key.KEY_BACKSPACE =>
        dispatch(Backspace);
        onChange(~value=removeCharacter(state.value));
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

    let hasPlaceholder = String.length(state.value) < 1;
    let content = hasPlaceholder ? placeholder : state.value;

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
          opacity(state.isFocused ? animatedOpacity : 0.0),
          backgroundColor(cursorColor),
        ]
      />;

    let inputText =
      <Text
        text=content
        style=Style.[
          color(hasPlaceholder ? placeholderColor : inputColor),
          fontFamily(inputFontFamily),
          fontSize(inputFontSize),
          alignItems(`Center),
          justifyContent(`FlexStart),
          marginLeft(hasPlaceholder ? 2 : inputTextMargin),
        ]
      />;

    (
      /*
         component
       */
      slots,
      <Clickable
        onFocus={() => dispatch(SetFocus(true))}
        onBlur={() => dispatch(SetFocus(false))}
        onKeyDown={handleKeyDown(~dispatch)}
        onKeyPress={
          event => {
            dispatch(UpdateText(event.character));
            onChange(~value=addCharacter(state.value, event.character));
          }
        }>
        <View style=viewStyles>
          ...{hasPlaceholder ? [cursor, inputText] : [inputText, cursor]}
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
      ~onChange=noop,
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
