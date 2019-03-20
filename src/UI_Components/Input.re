open Revery_UI;
open Revery_Core;

type state = {
  isFocused: bool,
  internalValue: string,
};

type action =
  | SetFocus(bool)
  | SetValue(string);

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
  | SetValue(internalValue) => {...state, internalValue}
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
      ~style,
      ~placeholder,
      ~cursorColor,
      ~placeholderColor,
      ~onChange,
      ~value as valueAsProp,
      (),
    ) =>
  component(slots => {
    let (state, dispatch, slots) =
      React.Hooks.reducer(
        ~initialState={isFocused: false, internalValue: ""},
        reducer,
        slots,
      );

    let slots =
      React.Hooks.effect(
        If(
          (oldValueAsProps, newValueAsProp) =>
            switch (oldValueAsProps, newValueAsProp) {
            | (None, Some(_)) => true
            | _ => false
            },
          valueAsProp,
        ),
        () => {
          dispatch(SetValue(""));
          Some(() => ());
        },
        slots,
      );

    let valueToDisplay =
      switch (valueAsProp) {
      | Some(v) => v
      | None => state.internalValue
      };

    let handleKeyDown = (event: NodeEvents.keyEventParams) =>
      switch (event.key) {
      | Key.KEY_BACKSPACE =>
        switch (valueAsProp) {
        | Some(v) => onChange(~value=removeCharacter(v))
        | None =>
          let newValue = removeCharacter(state.internalValue);
          dispatch(SetValue(newValue));
          onChange(~value=newValue);
        }

      | _ => ()
      };

    let handleKeyPress = (event: NodeEvents.keyPressEventParams) =>
      switch (valueAsProp) {
      | Some(v) => onChange(~value=addCharacter(v, event.character))
      | None =>
        let newValue = addCharacter(state.internalValue, event.character);
        dispatch(SetValue(newValue));
        onChange(~value=newValue);
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
        marginLeft(6),
      ];

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
        marginLeft(2),
        height(inputFontSize),
        width(2),
        opacity(state.isFocused ? animatedOpacity : 0.0),
        backgroundColor(cursorColor),
      ]
      |> (
        initial =>
          hasPlaceholder
            ? Style.[
                position(`Absolute),
                top(positionTop),
                left(5),
                ...initial,
              ]
            : initial
      );

    /*
       component
     */
    (
      slots,
      <Clickable
        onFocus={() => dispatch(SetFocus(true))}
        onBlur={() => dispatch(SetFocus(false))}
        onKeyDown=handleKeyDown
        onKeyPress=handleKeyPress>
        <View style=viewStyles>
          <Text style=innerTextStyles text=content />
          <View style=inputCursorStyles />
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
      ~placeholder="",
      ~onChange=noop,
      ~value=?,
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
