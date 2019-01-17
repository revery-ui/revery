open Revery_UI;
open Revery_Core;

let noop = () => ();

let textStyle = (~fontSize, ~fontFamily) =>
  Style.make(~fontSize, ~fontFamily, ~color=Colors.white, ());

let containerStyle = (~width, ~height, ~disabled, ~color) =>
  Style.make(
    ~position=Relative,
    ~backgroundColor=disabled ? Colors.dimGrey : color,
    ~justifyContent=JustifyCenter,
    ~alignItems=AlignCenter,
    ~border=Style.Border.make(~width=1, ~color=Colors.white, ()),
    ~height,
    ~width,
    (),
  );

let component = React.component("Button");

let make =
    (
      ~title,
      ~onClick=noop,
      ~color=Colors.dodgerBlue,
      ~fontSize=40,
      ~width=300,
      ~height=100,
      ~disabled=false,
      ~tabindex=?,
      ~onFocus=?,
      ~onBlur=?,
      ~fontFamily="Roboto-Regular.ttf",
      (),
    ) =>
  /* children, */
  component((_slots: React.Hooks.empty) =>
    <Clickable onClick={disabled ? noop : onClick} ?onFocus ?onBlur ?tabindex>
      <View style={containerStyle(~width, ~height, ~disabled, ~color)}>
        <Text style={textStyle(~fontSize, ~fontFamily)} text=title />
      </View>
    </Clickable>
  );

let createElement =
    (
      ~children as _,
      ~title,
      ~onClick=noop,
      ~color=Colors.dodgerBlue,
      ~fontSize=40,
      ~width=300,
      ~height=100,
      ~onFocus=?,
      ~onBlur=?,
      ~tabindex=?,
      ~disabled=false,
      ~fontFamily="Roboto-Regular.ttf",
      (),
    ) => {
  React.element(
    make(
      ~title,
      ~onClick,
      ~color,
      ~fontSize,
      ~width,
      ~height,
      ~disabled,
      ~fontFamily,
      ~onFocus?,
      ~onBlur?,
      ~tabindex?,
      (),
    ),
  );
};
