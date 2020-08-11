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

gr_glinterface_t *reason_skia_make_sdl2_gl_interface() {
    gr_glinterface_t *interface =
            gr_glinterface_assemble_gl_interface(0, reason_skia_sdl2_get);
    return interface;
}

gr_glinterface_t *reason_skia_make_sdl2_gles_interface() {
    gr_glinterface_t *interface =
            gr_glinterface_assemble_gles_interface(0, reason_skia_sdl2_get);
    return interface;
}

sk_shader_t* reason_skia_stub_linear_gradient2(
    sk_point_t* startPosition,
    sk_point_t* stopPosition,
    sk_color_t startColor,
    sk_color_t stopColor,
    sk_shader_tilemode_t tileMode) {

    sk_point_t pts[2];
    pts[0] = *startPosition;
    pts[1] = *stopPosition;

    sk_color_t colors[2];
    colors[0] = startColor;
    colors[1] = stopColor;

    float stops[2];
    stops[0] = 0.0f;
    stops[1] = 1.0f;

    return sk_shader_new_linear_gradient(
               pts,
               colors,
               stops,
               2,
               tileMode,
               NULL
           );
}

sk_shader_t* reason_skia_stub_linear_gradient(
    sk_point_t* startPosition,
    sk_point_t* stopPosition,
    sk_color_t* colors,
    float* positions,
    int count,
    sk_shader_tilemode_t tileMode) {

    sk_point_t pts[2];
    pts[0] = *startPosition;
    pts[1] = *stopPosition;

    return sk_shader_new_linear_gradient(
               pts,
               colors,
               positions,
               count,
               tileMode,
               NULL
           );
}
