#include "gr_context.h"
#include "sk_canvas.h"
#include "sk_paint.h"
#include "sk_shader.h"
#include "sk_types.h"

#include <SDL2/SDL.h>

void reason_skia_stub_sk_canvas_draw_rect_ltwh(sk_canvas_t *canvas, float left,
        float top, float width,
        float height, sk_paint_t *paint);

gr_glinterface_t *reason_skia_make_sdl2_gl_interface();
gr_glinterface_t *reason_skia_make_sdl2_gles_interface();

sk_shader_t* reason_skia_stub_linear_gradient2(
    sk_point_t* startPosition,
    sk_point_t* stopPosition,
    sk_color_t startColor,
    sk_color_t stopColor,
    sk_shader_tilemode_t tileMode);

sk_shader_t* reason_skia_stub_linear_gradient(
    sk_point_t* startPosition,
    sk_point_t* stopPosition,
    sk_color_t* colors,
    float* positions,
    int count,
    sk_shader_tilemode_t tileMode);
