/* Simple CLI util to test out the findFont functionality */
open Arg;

let fontFamily: ref(string) = ref("Arial");
let isBold: ref(bool) = ref(false);
let isItalic: ref(bool) = ref(false);
let isMono: ref(bool) = ref(false);

let spec = [
  ("-family", Set_string(fontFamily), "Font family to locate"),
  ("-bold", Set(isBold), "Search for bold version of font"),
  ("-italic", Set(isItalic), "Search for italic verison of font"),
  ("-mono", Set(isMono), "Search for monospace version of font"),
];

Arg.parse(spec, _ => (), "Simple CLI utility to test findFont");

FontManager.findFont(
  ~family=fontFamily^,
  ~weight=
    isBold^ ? FontManager.FontWeight.Bold : FontManager.FontWeight.Normal,
  ~italic=isItalic^,
  ~mono=isMono^,
  (),
)
|> FontManager.FontDescriptor.show
|> print_endline;
