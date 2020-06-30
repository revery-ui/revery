#include <stdio.h>

#include <caml/alloc.h>
#include <caml/bigarray.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#include <hb-ot.h>
#include <hb.h>

#include "ctypes_cstubs_internals.h"

/* #define TEST_FONT "E:/FiraCode-Regular.ttf" */
/* #define TEST_FONT "E:/Hasklig-Medium.otf" */
/* #define TEST_FONT "E:/Cassandra.ttf" */
/* #define TEST_FONT "E:/Lato-Regular.ttf" */

extern "C" {

// hb_font_t*

CAMLprim value Val_success(value v) {
  CAMLparam1(v);
  CAMLlocal1(some);
  some = caml_alloc(1, 0);
  Store_field(some, 0, v);
  CAMLreturn(some);
}

CAMLprim value Val_error(const char *szMsg) {
  CAMLparam0();
  CAMLlocal1(error);
  error = caml_alloc(1, 1);
  Store_field(error, 0, caml_copy_string(szMsg));
  CAMLreturn(error);
}

/* Use native open type implementation to load font
  https://github.com/harfbuzz/harfbuzz/issues/255 */
hb_font_t *get_font_ot(char *data, int length, int size) {
  hb_blob_t *blob =
      hb_blob_create(data, length, HB_MEMORY_MODE_WRITABLE, (void *)data, free);
  hb_face_t *face = hb_face_create(blob, 0);

  hb_blob_destroy(blob); // face will keep a reference to blob

  hb_font_t *font = hb_font_create(face); 
  hb_face_destroy(face); // font will keep a reference to face

  hb_ot_font_set_funcs(font);
  hb_font_set_scale(font, size, size);

  return font;
}
CAMLprim value rehb_destroy_face(value vFont) {
  CAMLparam1(vFont);

  hb_font_t *pFont = (hb_font_t*)vFont;
  if (pFont) {
    hb_font_destroy(pFont);
  }
  
  CAMLreturn(Val_unit);
}

CAMLprim value rehb_face_from_path(value vString) {
  CAMLparam1(vString);
  CAMLlocal1(ret);

  const char *szFont = String_val(vString);
  
  FILE *file = fopen(szFont, "rb");

  if (!file) {
    CAMLreturn(Val_error("File does not exist"));
  }

  fseek(file, 0, SEEK_END);
  unsigned int length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *data = (char *)malloc(length);
  fread(data, length, 1, file);
  fclose(file);

  hb_font_t *hb_font;
  hb_font = get_font_ot(data, length, 12 /*iSize*/ * 64);

  if (!hb_font) {
    ret = Val_error("Unable to load font");
  } else {
    ret = Val_success((value)hb_font);
  }
  CAMLreturn(ret);
}

CAMLprim value rehb_face_from_bytes(value vPtr, value vLength) {
  CAMLparam2(vPtr, vLength);
  CAMLlocal1(ret);

  char *data = Bp_val(vPtr);
  int length = Int_val(vLength);

  hb_font_t *hb_font;
  hb_font = get_font_ot(data, length, 12 /*iSize*/ * 64);

  if (!hb_font) {
    ret = Val_error("Unable to load font");
  } else {

    ret = Val_success((value)hb_font);
  }
  CAMLreturn(ret);
}

static value createShapeTuple(unsigned int codepoint, unsigned int cluster) {
  CAMLparam0();

  CAMLlocal1(ret);
  ret = caml_alloc(2, 0);
  Store_field(ret, 0, Val_int(codepoint));
  Store_field(ret, 1, Val_int(cluster));
  CAMLreturn(ret);
}

CAMLprim value rehb_shape(value vFace, value vString) {
  CAMLparam2(vFace, vString);
  CAMLlocal1(ret);

  hb_font_t *hb_font = (hb_font_t *)vFace;

  hb_buffer_t *hb_buffer;
  hb_buffer = hb_buffer_create();
  hb_buffer_add_utf8(hb_buffer, String_val(vString), -1, 0, -1);
  hb_buffer_guess_segment_properties(hb_buffer);

  unsigned int len = hb_buffer_get_length(hb_buffer);
  hb_glyph_info_t *info = hb_buffer_get_glyph_infos(hb_buffer, NULL);

  ret = caml_alloc(len, 0);

  hb_shape(hb_font, hb_buffer, NULL, 0);
  for (int i = 0; i < len; i++) {
    Store_field(ret, i, createShapeTuple(info[i].codepoint, info[i].cluster));
  }
  hb_buffer_destroy(hb_buffer);
  CAMLreturn(ret);
}
}
