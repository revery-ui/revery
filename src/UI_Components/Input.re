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

let textStyles = (~color, ~fontSize) =>
  Style.make(
    ~color,
    ~fontFamily="Roboto-Regular.ttf",
    ~fontSize,
    ~alignItems=LayoutTypes.AlignCenter,
    ~justifyContent=LayoutTypes.JustifyFlexStart,
    (),
  );

let cursorStyles = (~fontSize, ~backgroundColor, ~opacity) =>
  Style.make(~height=fontSize, ~width=2, ~backgroundColor, ~opacity, ());

type state = {
  value: string,
  placeholder: string,
};

type action =
  | UpdateText(string)
  | Backspace
  | ClearWord;

let reducer = (state, action) =>
  switch (action) {
  | UpdateText(t) => {...state, value: state.value ++ t}
  | Backspace =>
    let length = String.length(state.value);
    length > 0 ?
      {...state, value: String.sub(state.value, 0, length - 1)} : state;
  | ClearWord => {...state, value: ""}
  };

let handleKeyDown = (~dispatch, ~onSubmit, event: Events.keyEvent) =>
  switch (event.key) {
  | Key.KEY_BACKSPACE => dispatch(Backspace)
  | Key.KEY_ENTER => onSubmit(event)
  | _ => ()
  };

let handleKeyPress = (dispatch, event: Events.keyPressEvent) =>
  String.length(event.character) > 0 ?
    dispatch(UpdateText(event.character)) : ();

let noop = _evt => ();

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
                  ~onSubmit=noop,
                  (),
                ) =>
                render(
                  () => {
                    let ({value, placeholder}, dispatch) =
                      useReducer(reducer, {value: "", placeholder: ""});

                    useEffect(() =>
                      Event.subscribe(
                        window.onKeyPress,
                        handleKeyPress(dispatch),
                      )
                    );

                    useEffect(() =>
                      Event.subscribe(
                        window.onKeyDown,
                        handleKeyDown(~dispatch, ~onSubmit),
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

                    let content =
                      String.length(value) < 1 ? placeholder : value;

                    <view style={styles(~height, ~width)}>
                      <text style={textStyles(~color, ~fontSize)}>
                        content
                      </text>
                      <view
                        style={cursorStyles(
                          ~fontSize,
                          ~backgroundColor=color,
                          ~opacity,
                        )}
                      />
                    </view>;
                  },
                  ~children,
                )
              )
        );
