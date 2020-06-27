let prefix = "skia_wrapped_stub"

let prologue = "
#include \"include/c/sk_types.h\"
"

let () =
  print_endline prologue;
  Cstubs.Types.write_c Format.std_formatter (module SkiaWrappedTypes.M)
