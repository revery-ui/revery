open Revery_Core;
open Revery_UI;
open Revery_UI_Primitives;

let%component make =
              (
                ~children,
                ~color=Colors.transparentWhite,
                ~width as w,
                ~height as h,
                (),
              ) => {
  let c = color;
  let style =
    Style.[width(w), height(h), overflow(`Hidden), backgroundColor(c)];

  <View style> ...children </View>;
};
