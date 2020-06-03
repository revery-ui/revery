open Revery_UI;
open Revery_Core;
open Revery_UI_Primitives;
open Revery_Font;

let noop = () => ();

let make =
    (
      ~title,
      ~onClick=noop,
      ~color as c=Colors.dodgerBlue,
      ~fontSize=40.,
      ~width as w=300,
      ~height as h=100,
      ~disabled=false,
      ~tabindex=?,
      ~onFocus=?,
      ~onBlur=?,
      ~fontFamily=Family.default,
      (),
    ) =>
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
        style=Style.[color(Colors.white)]
        fontSize
        fontFamily
        text=title
      />
    </View>
  </Clickable>;
