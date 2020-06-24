/**
    Discovery.re

*/
open FontManager;

let find =
    (
      ~weight=FontWeight.Bold,
      ~width=FontWidth.Undefined,
      ~mono=false,
      ~italic=false,
      family,
    ) =>
  FontManager.findFontExn(~weight, ~width, ~mono, ~italic, ~family, ());

let toString = FontManager.FontDescriptor.show;
