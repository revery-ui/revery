open Revery_UI;
open Revery_Core;
open Revery_Core.Window;

let styles = (~height, ~width) =>
  Style.make(
    ~color=Colors.transparentWhite,
    ~height,
    ~width,
    ~flexDirection=LayoutTypes.Row,
    ~alignItems=LayoutTypes.AlignCenter,
    ~justifyContent=LayoutTypes.JustifyFlexStart,
    ~overflow=LayoutTypes.Hidden,
    ~border=
      Style.Border.make(
        /*
           The default border width should be 5% of the full input height
         */
        ~width=float_of_int(height) *. 0.05 |> int_of_float,
        ~color=Colors.black,
        (),
      ),
    (),
  );

let textStyles = (~color, ~fontSize, ~hasPlaceholder) =>
  Style.make(
    ~color=hasPlaceholder ? Colors.grey : color,
    ~fontFamily="Roboto-Regular.ttf",
    ~fontSize,
    ~alignItems=LayoutTypes.AlignCenter,
    ~justifyContent=LayoutTypes.JustifyFlexStart,
    ~marginLeft=6,
    (),
  );

let cursorStyles =
    (~fontSize, ~backgroundColor, ~opacity, ~containerHeight, ~hasPlaceholder) => {
  /*
     calculate the top padding needed to place the cursor centrally
   */
  let verticalAlignPos = (containerHeight - fontSize) / 2;
  let initialStyles =
    Style.make(
      ~marginLeft=2,
      ~height=fontSize,
      ~width=2,
      ~backgroundColor,
      ~opacity,
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

let reducer = (state, action) =>
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

include (
          val component(
                (
                  render,
                  ~children,
                  ~window,
                  ~color=Colors.black,
                  ~fontSize=18,
                  ~width=200,
                  ~height=50,
                  ~value=None,
                  ~placeholder="",
                  ~onChange=noop,
                  (),
                ) =>
                render(
                  () => {
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
                    let (state, dispatch) =
                      useReducer(reducer, initialState);

                    let valueToUse =
                      isControlled ? inheritedValue : state.value;

                    useEffect(() =>
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
                      )
                    );

                    useEffect(() =>
                      Event.subscribe(
                        window.onKeyDown,
                        event => {
                          if (!isControlled) {
                            handleKeyDown(~dispatch, event);
                          };
                          onChange(~value=removeCharacter(state.value));
                        },
                      )
                    );

                    let opacity =
                      useAnimation(
                        Animated.floatValue(0.),
                        {
                          toValue: 1.,
                          duration: Seconds(0.5),
                          delay: Seconds(0.),
                          repeat: true,
                          easing: Animated.linear,
                        },
                      );

                    let hasPlaceholder = String.length(valueToUse) < 1;

                    let content =
                      hasPlaceholder ? state.placeholder : valueToUse;

                    <view style={styles(~height, ~width)}>
                      <text
                        style={textStyles(~color, ~fontSize, ~hasPlaceholder)}>
                        content
                      </text>
                      /*
                         TODO:
                         1. Passed valued does not update correctly (reconciler?)
                         2. Show and hide cursor based on focus
                         3. Add Focus
                         4. Add Mouse events
                       */
                      <view
                        style={cursorStyles(
                          ~opacity,
                          ~fontSize,
                          ~backgroundColor=color,
                          ~containerHeight=height,
                          ~hasPlaceholder,
                        )}
                      />
                    </view>;
                  },
                  ~children,
                )
              )
        );
