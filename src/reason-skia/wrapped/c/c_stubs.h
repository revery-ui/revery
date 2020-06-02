#include "gr_context.h"
#include "sk_canvas.h"
#include "sk_paint.h"
#include "sk_types.h"

#include <SDL2/SDL.h>

void reason_skia_stub_sk_canvas_draw_rect_ltwh(sk_canvas_t *canvas, float left,
                                               float top, float width,
                                               float height, sk_paint_t *paint);

gr_glinterface_t *reason_skia_make_sdl2_interface();
