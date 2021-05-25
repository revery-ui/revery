/**
    Discovery.re

*/
exception Font_not_found;

let find =
    (
      ~weight=FontWeight.Normal,
      ~width=FontWidth.Normal,
      ~italic=false,
      family,
    ) => {
  let style =
    Skia.FontStyle.make(
      FontWeight.toInt(weight),
      FontWidth.toInt(width),
      italic ? `Italic : `Upright,
    );
  Skia.FontManager.matchFamilyStyle(FontManager.instance, family, style);
};
