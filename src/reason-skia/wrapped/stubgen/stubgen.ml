let prefix = "skia_wrapped_stub"

let prologue = "
#include \"c_stubs.h\"
#include \"include/c/gr_context.h\"
#include \"include/c/sk_canvas.h\"
#include \"include/c/sk_data.h\"
#include \"include/c/sk_image.h\"
#include \"include/c/sk_imagefilter.h\"
#include \"include/c/sk_paint.h\"
#include \"include/c/sk_path.h\"
#include \"include/c/sk_surface.h\"
#include \"include/c/sk_rrect.h\"
#include \"include/c/sk_matrix.h\"
#include \"include/c/sk_typeface.h\"
#include \"include/c/sk_font.h\"
#include \"include/c/sk_stream.h\"
"

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
