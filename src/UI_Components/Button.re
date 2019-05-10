open Revery_UI;
open Revery_Core;
open Revery_UI_Primitives;

let noop = () => ();

let component = React.component("Button");

let createElement =
    (
      ~children as _,
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
  component(slots =>
    (
      slots,
      <Clickable
        onClick={disabled ? noop : onClick} ?onFocus ?onBlur ?tabindex>
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
      </Clickable>,
    )
  );
