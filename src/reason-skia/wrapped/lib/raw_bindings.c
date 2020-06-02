
/*
 * Use this file for any manual, raw bindings - ie,
 * ones that use [@noalloc], [@unboxed].
 */

#include "c_stubs.h"
#include "sk_canvas.h"
#include "sk_matrix.h"
#include "sk_paint.h"
#include "sk_types.h"

#include <stdio.h>
#include <stdlib.h>

#include "ctypes_cstubs_internals.h"

CAMLprim value reason_skia_paint_set_color(value vPaint, int32_t color) {
  sk_paint_t *pPaint = CTYPES_ADDR_OF_FATPTR(vPaint);
  sk_paint_set_color(pPaint, color);
  return Val_unit;
}

CAMLprim value reason_skia_paint_set_alphaf(value vPaint, double alpha) {
  sk_paint_t *pPaint = CTYPES_ADDR_OF_FATPTR(vPaint);

  int a = (int)(255.0 * alpha);
  sk_color_t c = sk_paint_get_color(pPaint);

  sk_paint_set_color(pPaint,
                     sk_color_set_argb(a, sk_color_get_r(c), sk_color_get_g(c),
                                       sk_color_get_b(c)));

  sk_color_t newColor = sk_paint_get_color(pPaint);

  return Val_unit;
}

CAMLprim value reason_skia_paint_set_alphaf_byte(value vPaint, value vAlpha) {
  reason_skia_paint_set_alphaf(vPaint, Double_val(vAlpha));
  return Val_unit;
}

CAMLprim value reason_skia_paint_set_color_byte(value vPaint, value vColor) {
  reason_skia_paint_set_color(vPaint, Int32_val(vColor));

  return Val_unit;
}

double reason_skia_stub_sk_color_float_get_b(int32_t color) {
  return (double)(sk_color_get_b(color) / 255.0);
}

CAMLprim value reason_skia_stub_sk_color_float_get_b_byte(value vColor) {
  return caml_copy_double(
      reason_skia_stub_sk_color_float_get_b(Int32_val(vColor)));
}

double reason_skia_stub_sk_color_float_get_g(int32_t color) {
  return (double)(sk_color_get_g(color) / 255.0);
}

CAMLprim value reason_skia_stub_sk_color_float_get_g_byte(value vColor) {
  return caml_copy_double(
      reason_skia_stub_sk_color_float_get_g(Int32_val(vColor)));
}

double reason_skia_stub_sk_color_float_get_r(int32_t color) {
  return (double)(sk_color_get_r(color) / 255.0);
}

CAMLprim value reason_skia_stub_sk_color_float_get_r_byte(value vColor) {
  return caml_copy_double(
      reason_skia_stub_sk_color_float_get_r(Int32_val(vColor)));
}

double reason_skia_stub_sk_color_float_get_a(int32_t color) {
  return (double)(sk_color_get_a(color) / 255.0);
}

CAMLprim value reason_skia_stub_sk_color_float_get_a_byte(value vColor) {
  return caml_copy_double(
      reason_skia_stub_sk_color_float_get_a(Int32_val(vColor)));
}

uint32_t reason_skia_color_float_make_argb(double a, double r, double g,
                                           double b) {
  int iA = (int)(255.0 * a);
  int iR = (int)(255.0 * r);
  int iG = (int)(255.0 * g);
  int iB = (int)(255.0 * b);
  return (uint32_t)sk_color_set_argb(iA, iR, iG, iB);
}

CAMLprim value reason_skia_color_float_make_argb_byte(value vA, value vR,
                                                      value vG, value vB) {
  return caml_copy_int32(reason_skia_color_float_make_argb(
      Double_val(vA), Double_val(vR), Double_val(vG), Double_val(vB)));
}

uint32_t reason_skia_stub_sk_color_get_a(int32_t color) {
  return (uint32_t)sk_color_get_a(color);
}

CAMLprim value reason_skia_stub_sk_color_get_a_byte(value vColor) {
  return caml_copy_int32(reason_skia_stub_sk_color_get_a(Int32_val(vColor)));
}

uint32_t reason_skia_stub_sk_color_get_r(int32_t color) {
  return (uint32_t)sk_color_get_r(color);
}

CAMLprim value reason_skia_stub_sk_color_get_r_byte(value vColor) {
  return caml_copy_int32(reason_skia_stub_sk_color_get_r(Int32_val(vColor)));
}

uint32_t reason_skia_stub_sk_color_get_g(int32_t color) {
  return (uint32_t)sk_color_get_g(color);
}

CAMLprim value reason_skia_stub_sk_color_get_g_byte(value vColor) {
  return caml_copy_int32(reason_skia_stub_sk_color_get_g(Int32_val(vColor)));
}

uint32_t reason_skia_stub_sk_color_get_b(int32_t color) {
  return (uint32_t)sk_color_get_b(color);
}

CAMLprim value reason_skia_stub_sk_color_get_b_byte(value vColor) {
  return caml_copy_int32(reason_skia_stub_sk_color_get_b(Int32_val(vColor)));
}

uint32_t reason_skia_stub_sk_color_set_argb(int32_t alpha, int32_t red,
                                            int32_t green, int32_t blue) {
  return (uint32_t)sk_color_set_argb(alpha, red, green, blue);
}

CAMLprim value reason_skia_stub_sk_color_set_argb_byte(value vAlpha, value vRed,
                                                       value vGreen,
                                                       value vBlue) {
  return caml_copy_int32(reason_skia_stub_sk_color_set_argb(
      Int32_val(vAlpha), Int32_val(vRed), Int32_val(vGreen), Int32_val(vBlue)));
}

double reason_skia_rect_get_bottom(value vRect) {
  sk_rect_t *pRect = CTYPES_ADDR_OF_FATPTR(vRect);
  return (double)pRect->bottom;
}

CAMLprim value reason_skia_rect_get_bottom_byte(value vRect) {
  return caml_copy_double(reason_skia_rect_get_bottom(vRect));
}

double reason_skia_rect_get_right(value vRect) {
  sk_rect_t *pRect = CTYPES_ADDR_OF_FATPTR(vRect);
  return (double)pRect->right;
}

CAMLprim value reason_skia_rect_get_right_byte(value vRect) {
  return caml_copy_double(reason_skia_rect_get_right(vRect));
}

double reason_skia_rect_get_top(value vRect) {
  sk_rect_t *pRect = CTYPES_ADDR_OF_FATPTR(vRect);
  return (double)pRect->top;
}

CAMLprim value reason_skia_rect_get_top_byte(value vRect) {
  return caml_copy_double(reason_skia_rect_get_top(vRect));
}

double reason_skia_rect_get_left(value vRect) {
  sk_rect_t *pRect = CTYPES_ADDR_OF_FATPTR(vRect);
  return (double)pRect->left;
}

CAMLprim value reason_skia_rect_get_left_byte(value vRect) {
  return caml_copy_double(reason_skia_rect_get_left(vRect));
}

CAMLprim value reason_skia_rect_set(value vRect, double left, double top,
                                    double right, double bottom) {
  sk_rect_t *pRect = CTYPES_ADDR_OF_FATPTR(vRect);
  pRect->left = left;
  pRect->top = top;
  pRect->right = right;
  pRect->bottom = bottom;

  return Val_unit;
}

CAMLprim value reason_skia_rect_set_byte(value vRect, value vLeft, value vTop,
                                         value vRight, value vBottom) {
  return reason_skia_rect_set(vRect, Double_val(vLeft), Double_val(vTop),
                              Double_val(vRight), Double_val(vBottom));
}

CAMLprim value reason_skia_matrix_set_scale(value vMatrix, double scaleX,
                                            double scaleY, double pivotX,
                                            double pivotY) {
  float *pMatrix = CTYPES_ADDR_OF_FATPTR(vMatrix);
  pMatrix[0] = scaleX;
  pMatrix[1] = 0.0;
  pMatrix[2] = pivotX - (scaleX * pivotX);
  pMatrix[3] = 0.0;
  pMatrix[4] = scaleY;
  pMatrix[5] = pivotY - (scaleY * pivotY);
  pMatrix[6] = 0.0;
  pMatrix[7] = 0.0;
  pMatrix[8] = 1.0;
  return Val_unit;
}

CAMLprim value reason_skia_matrix_set_scale_byte(value vMatrix, value vScaleX,
                                                 value vScaleY, value vPivotX,
                                                 value vPivotY) {
  return reason_skia_matrix_set_scale(vMatrix, Double_val(vScaleX),
                                      Double_val(vScaleY), Double_val(vPivotX),
                                      Double_val(vPivotY));
}

CAMLprim value reason_skia_matrix_set_translate(value vMatrix,
                                                double translateX,
                                                double translateY) {
  float *pMatrix = CTYPES_ADDR_OF_FATPTR(vMatrix);
  pMatrix[0] = 1.0;
  pMatrix[1] = 0.0;
  pMatrix[2] = translateX;
  pMatrix[3] = 0.0;
  pMatrix[4] = 1.0;
  pMatrix[5] = translateY;
  pMatrix[6] = 0.0;
  pMatrix[7] = 0.0;
  pMatrix[8] = 1.0;
  return Val_unit;
}

CAMLprim value reason_skia_matrix_set_translate_byte(value vMatrix,
                                                     value vTranslateX,
                                                     value vTranslateY) {
  return reason_skia_matrix_set_translate(vMatrix, Double_val(vTranslateX),
                                          Double_val(vTranslateY));
}
