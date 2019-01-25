open Revery_UI;
open Revery_Core;
open Revery_Core.Window;

type state = {
  value: string,
  placeholder: string,
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
    state.isFocused ? {...state, value: addCharacter(state.value, t)} : state
  | Backspace =>
    state.isFocused
      ? {
        let length = String.length(state.value);
        length > 0 ? {...state, value: removeCharacter(state.value)} : state;
      }
      : state
  | ClearWord => {...state, value: ""}
  };

let handleKeyDown = (~dispatch, event: Events.keyEvent) =>
  switch (event.key) {
  | Key.KEY_BACKSPACE => dispatch(Backspace)
  | _ => ()
  };

let noop = (~value as _value) => ();

let defaultStyles =
  Style.[
    color(Colors.black),
    width(200),
    height(50),
    border(
      /*
         The default border width should be 5% of the full input height
       */
      ~width=float_of_int(50) *. 0.05 |> int_of_float,
      ~color=Colors.black,
    ),
    backgroundColor(Colors.transparentWhite),
  ];

let component = React.component("Input");
let make =
    (
      ~window,
      ~style,
      ~value,
      ~placeholder,
      ~cursorColor,
      ~placeholderColor,
      ~onChange,
      (),
    ) =>
  component(slots => {
    let initialState = {value, placeholder, isFocused: false};
    let (state, dispatch, slots) =
      React.Hooks.reducer(~initialState, reducer, slots);

    /*
       TODO: Setting the hook to run only on mount means that the onChange
       handler is only called with a stale version of state
       BUT setting the value to ALWAYS causes one handler to to seize
       control of the event listening
     */
    let slots =
      React.Hooks.effect(
        OnMount,
        () =>
          Some(
            Event.subscribe(
              window.onKeyPress,
              event => {
                dispatch(UpdateText(event.character));
                onChange(~value);
              },
            ),
          ),
        slots,
      );

    let slots =
      React.Hooks.effect(
        OnMount,
        () =>
          Some(
            Event.subscribe(
              window.onKeyDown,
              event => {
                handleKeyDown(~dispatch, event);
                onChange(~value=state.value);
              },
            ),
          ),
        slots,
      );

    let (animatedOpacity, _slots: React.Hooks.empty) =
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

    let content = hasPlaceholder ? state.placeholder : state.value;

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
            overflow(LayoutTypes.Hidden),
            cursor(MouseCursors.text),
            ...defaultStyles,
          ],
          ~target=style,
        )
      );

    let viewStyles = Style.extractViewStyles(allStyles);

    let inputHeight =
      List.fold_left(
        (default, s) =>
          switch (s) {
          | `Height(h) => h
          | _ => default
          },
        18,
        style,
      );

    let inputFontSize =
      List.fold_left(
        (default, s) =>
          switch (s) {
          | `FontSize(fs) => fs
          | _ => default
          },
        20,
        style,
      );

    let inputColor =
      List.fold_left(
        (default, s) =>
          switch (s) {
          | `Color(c) => c
          | _ => default
          },
        Colors.black,
        style,
      );

    let innerTextStyles =
      Style.[
        color(hasPlaceholder ? placeholderColor : inputColor),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(inputFontSize),
        alignItems(`Center),
        justifyContent(`FlexStart),
        marginLeft(6),
      ];

    /*
       calculate the top padding needed to place the cursor centrally
     */
    let verticalAlignPos = (inputHeight - 20) / 2;
    let inputCursorStyles =
      Style.[
        marginLeft(2),
        height(inputHeight),
        width(2),
        opacity(state.isFocused ? animatedOpacity : 0.0),
        backgroundColor(cursorColor),
      ]
      |> (
        initial =>
          hasPlaceholder
            ? Style.[
                position(`Absolute),
                top(verticalAlignPos),
                left(5),
                ...initial,
              ]
            : initial
      );

    /*
       component
     */
    <Clickable
      onFocus={() => dispatch(SetFocus(true))}
      onBlur={() => dispatch(SetFocus(false))}>
      <View style=viewStyles>
        <Text style=innerTextStyles text=content />
        <View style=inputCursorStyles />
      </View>
    </Clickable>;
  });

let createElement =
    (
      ~window,
      ~children as _,
      ~style=defaultStyles,
      ~placeholderColor=Colors.grey,
      ~cursorColor=Colors.white,
      ~value="",
      ~placeholder="",
      ~onChange=noop,
      (),
    ) =>
  React.element(
    make(
      ~window,
      ~value,
      ~style,
      ~placeholder,
      ~cursorColor,
      ~placeholderColor,
      ~onChange,
      (),
    ),
  );
