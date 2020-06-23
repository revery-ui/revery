/**
    Discovery.re

*/
open FontManager;

let callback_font_not_found =
  ref((~weight, ~width, ~italic, ~mono, family) =>
    raise(FontManager.Font_not_found)
  );

let find =
    (
      ~weight=FontWeight.Bold,
      ~width=FontWidth.Undefined,
      ~mono=false,
      ~italic=false,
      family,
    ) =>
  FontManager.findFont(~weight, ~width, ~mono, ~italic, ~family, ());

let findExn =
    (
      ~weight=FontWeight.Bold,
      ~width=FontWidth.Undefined,
      ~mono=false,
      ~italic=false,
      family,
    ) =>
  switch (FontManager.findFont(~weight, ~width, ~mono, ~italic, ~family, ())) {
  | Some(fd) => fd
  | None => callback_font_not_found^(~weight, ~width, ~mono, ~italic, family)
  };

let toString = FontManager.FontDescriptor.show;
