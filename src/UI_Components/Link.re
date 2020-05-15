open Revery_Native;

let make = (~text, ~inactiveStyle, ~activeStyle, ~href, ()) => {
  let onClick = _ => Shell.openURL(href) |> ignore;
  <TextButton text inactiveStyle activeStyle onClick />;
};
