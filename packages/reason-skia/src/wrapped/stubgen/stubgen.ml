let prefix = "skia_wrapped_stub"

let prologue = {|
#include "c_stubs.h"
#include "gr_context.h"
#include "sk_canvas.h"
#include "sk_data.h"
#include "sk_image.h"
#include "sk_imagefilter.h"
#include "sk_paint.h"
#include "sk_path.h"
#include "sk_surface.h"
#include "sk_rrect.h"
#include "sk_matrix.h"
#include "sk_typeface.h"
#include "sk_stream.h"
#include "sk_string.h"

// Enable the SVG functions
#define ESY_SKIA_SVG
#include "sk_svg.h"
|}

let () =
  let generate_ml, generate_c = ref false, ref false in
  let () =
    Arg.(parse [ ("-ml", Set generate_ml, "Generate ML");
                 ("-c", Set generate_c, "Generate C") ])
      (fun _ -> failwith "unexpected anonymous argument")
      "stubgen [-ml|-c]"
  in
  match !generate_ml, !generate_c with
  | false, false
  | true, true ->
    failwith "Exactly one of -ml and -c must be specified"
  | true, false ->
    Cstubs.write_ml Format.std_formatter ~prefix (module SkiaWrappedBindings.M)
  | false, true ->
    print_endline prologue;
    Cstubs.write_c Format.std_formatter ~prefix (module SkiaWrappedBindings.M)
