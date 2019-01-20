open Revery_UI;
open Revery_Core;
open Revery_Core.Window;

let containerStyles =
    (
      ~margin,
      ~marginLeft,
      ~marginRight,
      ~marginBottom,
      ~marginTop,
      ~top,
      ~right,
      ~left,
      ~bottom,
      ~height,
      ~width,
      ~border,
      ~color,
      ~backgroundColor,
      ~boxShadow,
      (),
    ) =>
  Style.make(
    ~margin,
    ~marginLeft,
    ~marginRight,
    ~marginBottom,
    ~marginTop,
    ~backgroundColor,
    ~width,
    ~top,
    ~right,
    ~left,
    ~bottom,
    ~color,
    ~height,
    ~flexDirection=LayoutTypes.Row,
    ~alignItems=LayoutTypes.AlignCenter,
    ~justifyContent=LayoutTypes.JustifyFlexStart,
    ~overflow=LayoutTypes.Hidden,
    ~border,
    ~boxShadow,
    ~cursor=MouseCursors.text,
    (),
  );

let textStyles = (~color, ~fontSize, ~hasPlaceholder, ~placeholderColor) =>
  Style.make(
    ~color=hasPlaceholder ? placeholderColor : color,
    ~fontFamily="Roboto-Regular.ttf",
    ~fontSize,
    ~alignItems=LayoutTypes.AlignCenter,
    ~justifyContent=LayoutTypes.JustifyFlexStart,
    ~marginLeft=6,
    (),
  );

let cursorStyles =
    (~fontSize, ~cursorColor, ~opacity, ~containerHeight, ~hasPlaceholder) => {
  /*
     calculate the top padding needed to place the cursor centrally
   */
  let verticalAlignPos = (containerHeight - fontSize) / 2;
  Style.make(
    ~marginLeft=2,
    ~height=fontSize,
    ~width=2,
    ~opacity,
    ~backgroundColor=cursorColor,
    (),
  )
  |> (
    initial =>
      hasPlaceholder ?
        Style.extend(
          initial,
          ~position=LayoutTypes.Absolute,
          ~top=verticalAlignPos,
          ~left=5,
          (),
        ) :
        initial
  );
};

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

let defaultBorder = height =>
  Style.Border.make(
    /*
       The default border width should be 5% of the full input height
     */
    ~width=float_of_int(height) *. 0.05 |> int_of_float,
    ~color=Colors.black,
    (),
  );

let defaultStyles =
  Style.make(
    ~fontSize=18,
    ~color=Colors.black,
    ~width=200,
    ~height=50,
    ~border=defaultBorder(50),
    ~backgroundColor=Colors.transparentWhite,
    (),
  );

let component = React.component("Input");
let make =
    (
      ~window,
      ~margin,
      ~marginLeft,
      ~marginRight,
      ~marginBottom,
      ~marginTop,
      ~boxShadow,
      ~height,
      ~width,
      ~top,
      ~bottom,
      ~right,
      ~left,
      ~color,
      ~backgroundColor,
      ~fontSize,
      ~border,
      ~value,
      ~placeholder,
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

    let (opacity, _slots: React.Hooks.empty) =
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
    let viewStyles =
      containerStyles(
        ~margin,
        ~marginLeft,
        ~marginRight,
        ~marginBottom,
        ~marginTop,
        ~height,
        ~width,
        ~color,
        ~backgroundColor,
        ~boxShadow,
        ~border,
        ~left,
        ~right,
        ~top,
        ~bottom,
        (),
      );

    let innerTextStyles =
      textStyles(~color, ~fontSize, ~hasPlaceholder, ~placeholderColor);

    let inputCursorStyles =
      cursorStyles(
        ~opacity=state.isFocused ? opacity : 0.0,
        ~fontSize,
        ~cursorColor=color,
        ~containerHeight=height,
        ~hasPlaceholder,
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
      ~margin=defaultStyles.margin,
      ~marginLeft=defaultStyles.marginLeft,
      ~marginRight=defaultStyles.marginRight,
      ~marginBottom=defaultStyles.marginBottom,
      ~marginTop=defaultStyles.marginTop,
      ~boxShadow=defaultStyles.boxShadow,
      ~height=defaultStyles.height,
      ~width=defaultStyles.width,
      ~top=defaultStyles.top,
      ~bottom=defaultStyles.bottom,
      ~right=defaultStyles.right,
      ~left=defaultStyles.left,
      ~color=defaultStyles.color,
      ~backgroundColor=defaultStyles.backgroundColor,
      ~placeholderColor=Colors.grey,
      ~fontSize=defaultStyles.fontSize,
      ~border=defaultStyles.border,
      ~value="",
      ~placeholder="",
      ~onChange=noop,
      (),
    ) =>
  React.element(
    make(
      ~window,
      ~margin,
      ~marginLeft,
      ~marginRight,
      ~marginBottom,
      ~marginTop,
      ~boxShadow,
      ~height,
      ~width,
      ~top,
      ~bottom,
      ~right,
      ~left,
      ~color,
      ~backgroundColor,
      ~fontSize,
      ~border,
      ~value,
      ~placeholder,
      ~placeholderColor,
      ~onChange,
      (),
    ),
  );
