open Revery_Native;
open Revery_Font;

let make =
    (
      ~text,
      ~inactiveStyle,
      ~activeStyle,
      ~href,
      ~fontSize=14.,
      ~fontFamily=Family.default,
      ~fontWeight=Weight.Normal,
      ~italic=false,
      ~monospaced=false,
      (),
    ) => {
  let onClick = _ => Shell.openURL(href) |> ignore;
  <ClickableText
    text
    inactiveStyle
    activeStyle
    onClick
    fontSize
    fontFamily
    fontWeight
    italic
    monospaced
  />;
};
