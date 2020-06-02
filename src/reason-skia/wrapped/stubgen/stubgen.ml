let prefix = "skia_wrapped_stub"

let prologue = "
#include \"gr_context.h\"
#include \"sk_canvas.h\"
#include \"sk_data.h\"
#include \"sk_image.h\"
#include \"sk_imagefilter.h\"
#include \"sk_paint.h\"
#include \"sk_types.h\"
#include \"sk_path.h\"
#include \"sk_surface.h\"
#include \"sk_rrect.h\"
#include \"sk_matrix.h\"
#include \"sk_typeface.h\"

void reason_skia_stub_sk_canvas_draw_rect_ltwh(sk_canvas_t *canvas, float left,
        float top, float width,
        float height, sk_paint_t *paint);

gr_glinterface_t *reason_skia_make_sdl2_interface();

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
