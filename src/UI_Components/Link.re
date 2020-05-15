open Revery_Native;

let make = (~text, ~inactiveStyle, ~activeStyle, ~href, ()) => {
  let onClick = _ => Shell.openURL(href) |> ignore;
  <ClickableText text inactiveStyle activeStyle onClick />;
};
