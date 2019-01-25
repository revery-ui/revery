open Revery_UI;
open Revery_Core;

let noop = () => ();

let component = React.component("Button");

let make =
    (
      ~title,
      ~onClick=noop,
      ~color as c=Colors.dodgerBlue,
      ~fontSize as size=40,
      ~width as w=300,
      ~height as h=100,
      ~disabled=false,
      ~tabindex=?,
      ~onFocus=?,
      ~onBlur=?,
      ~fontFamily as family="Roboto-Regular.ttf",
      (),
    ) =>
  /* children, */
  component((_slots: React.Hooks.empty) =>
    <Clickable onClick={disabled ? noop : onClick} ?onFocus ?onBlur ?tabindex>
      <View
        style=Style.[
          position(`Relative),
          backgroundColor(disabled ? Colors.dimGrey : c),
          justifyContent(`Center),
          alignItems(`Center),
          border(~width=1, ~color=Colors.white),
          height(h),
          width(w),
        ]>
        <Text
          style=Style.[
            fontSize(size),
            fontFamily(family),
            color(Colors.white),
          ]
          text=title
        />
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
    ) =>
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
