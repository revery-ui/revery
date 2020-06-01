/*
 * Use this file for building any C-layer functionality that we want to keep out
 * of Reason
 */

#include "c_stubs.h"

#include <stdio.h>
#include <stdlib.h>

void reason_skia_stub_sk_canvas_draw_rect_ltwh(sk_canvas_t *canvas, float left,
                                               float top, float width,
                                               float height,
                                               sk_paint_t *paint) {
  sk_rect_t rect;
  rect.left = left;
  rect.top = top;
  rect.right = left + width;
  rect.bottom = top + height;

  sk_canvas_draw_rect(canvas, &rect, paint);
}

void *reason_skia_sdl2_get(void *ctx, const char name[]) {
  return SDL_GL_GetProcAddress(name);
};

gr_glinterface_t *reason_skia_make_sdl2_interface() {
  gr_glinterface_t *interface =
      gr_glinterface_assemble_gl_interface(0, reason_skia_sdl2_get);
  return interface;
}
