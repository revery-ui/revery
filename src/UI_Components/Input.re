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
    ~top,
    ~right,
    ~left,
    ~bottom,
    ~color,
    ~height,
    ~width,
    ~flexDirection=LayoutTypes.Row,
    ~alignItems=LayoutTypes.AlignCenter,
    ~justifyContent=LayoutTypes.JustifyFlexStart,
    ~overflow=LayoutTypes.Hidden,
    ~border,
    ~boxShadow,
    (),
  );

let textStyles = (~color, ~width, ~fontSize, ~hasPlaceholder) => {
  let leftOffset = 6;
  Style.make(
    ~color=hasPlaceholder ? Colors.grey : color,
    ~width=width - leftOffset,
    ~fontFamily="Roboto-Regular.ttf",
    ~fontSize,
    ~alignItems=LayoutTypes.AlignCenter,
    ~justifyContent=LayoutTypes.JustifyFlexStart,
    ~marginLeft=leftOffset,
    (),
  );
};

let cursorStyles =
    (~fontSize, ~cursorColor, ~opacity, ~containerHeight, ~hasPlaceholder) => {
  /*
     calculate the top padding needed to place the cursor centrally
   */
  let verticalAlignPos = (containerHeight - fontSize) / 2;
  let initialStyles =
    Style.make(
      ~marginLeft=2,
      ~height=fontSize,
      ~width=2,
      ~opacity,
      ~backgroundColor=cursorColor,
      ~cursor=MouseCursors.text,
    );

  hasPlaceholder ?
    initialStyles(
      ~position=LayoutTypes.Absolute,
      ~top=verticalAlignPos,
      ~left=5,
      (),
    ) :
    initialStyles();
};

type state = {
  value: string,
  placeholder: string,
};

type action =
  | UpdateText(string)
  | Backspace
  | ClearWord;

let removeCharacter = word =>
  String.length(word)
  |> (length => length > 0 ? String.sub(word, 0, length - 1) : word);

let addCharacter = (word, char) => word ++ char;

let reducer = (action, state) =>
  switch (action) {
  | UpdateText(t) => {...state, value: addCharacter(state.value, t)}
  | Backspace =>
    let length = String.length(state.value);
    length > 0 ? {...state, value: removeCharacter(state.value)} : state;
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

module Input = {
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
        ~onChange,
        (),
      ) =>
    component(slots => {
      /*
         value is specified as option as it allows a more accurate check as to
         whether the component is controlled on uncontrolled. Using a length check
         does not work as well as changing the size of the value can lead to incorrect
         behaviour.
       */
      let (inheritedValue, isControlled) =
        switch (value) {
        | Some(v) => (v, true)
        | None => ("", false)
        };

      let initialState = {value: inheritedValue, placeholder};
      let (state, dispatch, slots) =
        React.Hooks.reducer(~initialState, reducer, slots);

      let (focused, setFocus, slots) = React.Hooks.state(true, slots);

      let valueToUse = isControlled ? inheritedValue : state.value;

      let slots =
        React.Hooks.effect(
          OnMount,
          () =>
            !focused ?
              None :
              Some(
                Event.subscribe(
                  window.onKeyPress,
                  event => {
                    if (!isControlled) {
                      dispatch(UpdateText(event.character));
                    };
                    onChange(
                      ~value=addCharacter(valueToUse, event.character),
                    );
                  },
                ),
              ),
          slots,
        );

      let slots =
        React.Hooks.effect(
          OnMount,
          () =>
            !focused ?
              None :
              Some(
                Event.subscribe(
                  window.onKeyDown,
                  event => {
                    if (!isControlled) {
                      handleKeyDown(~dispatch, event);
                    };
                    onChange(~value=removeCharacter(state.value));
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
            delay: Seconds(0.),
            repeat: true,
            easing: Animated.linear,
          },
          slots,
        );

      let hasPlaceholder = String.length(valueToUse) < 1;

      let content = hasPlaceholder ? state.placeholder : valueToUse;

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
        textStyles(~color, ~fontSize, ~width, ~hasPlaceholder);

      let inputCursorStyles =
        cursorStyles(
          ~opacity,
          ~fontSize,
          ~cursorColor=color,
          ~containerHeight=height,
          ~hasPlaceholder,
        );

      /*
         component
       */
      <Clickable onFocus={() => setFocus(!focused)}>
        <View style=viewStyles>
          <Text style=innerTextStyles text=content />
          /*
             TODO:
             1. Passed valued does not update correctly (reconciler?)
             2. Show and hide cursor based on focus
             3. Add Focus Management
             4. Add Mouse events
           */
          <View style=inputCursorStyles />
        </View>
      </Clickable>;
    });

  let createElement =
      (
        ~window,
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
        ~backgroundColor=defaultStyles.color,
        ~fontSize=defaultStyles.fontSize,
        ~border=defaultStyles.border,
        ~value=None,
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
        ~onChange,
        (),
      ),
    );
};
