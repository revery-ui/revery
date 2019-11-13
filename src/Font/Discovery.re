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
  FontManager.findFont(~weight, ~width, ~mono, ~italic, ~family, ());

let toString = FontManager.FontDescriptor.show;
