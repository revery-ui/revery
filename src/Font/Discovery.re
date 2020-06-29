/**
    Discovery.re

*/
open FontManager;

let callbackFontNotFound =
  ref((~weight as _, ~width as _, ~mono as _, ~italic as _, _) =>
    raise(FontManager.Font_not_found)
  );
let setFallbackResolver = newCallback => callbackFontNotFound := newCallback;
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
  | None => callbackFontNotFound^(~weight, ~width, ~mono, ~italic, family)
  };

let toString = FontManager.FontDescriptor.show;
