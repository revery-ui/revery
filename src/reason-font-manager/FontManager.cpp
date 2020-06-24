#include <caml/alloc.h>
#include <caml/bigarray.h>
#include <caml/callback.h>
#include <caml/fail.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/threads.h>
#include <stdio.h>

#include "FontDescriptor.h"

extern "C" {

extern FontDescriptor *findFont(FontDescriptor *query);
extern ResultSet *findFonts(FontDescriptor *query);

CAMLprim value fm_findFont(value family, value weight, value width, value italic, value monospace) {
  CAMLparam5(family, weight, width, italic, monospace);
  CAMLlocal1(ret);

  const char *fontFamily = String_val(family);
  FontWeight weightToCheck = (FontWeight)Int_val(weight);
  FontWidth widthToCheck = (FontWidth)Int_val(width);
  int isItalic = Bool_val(italic);
  int isMono = Bool_val(monospace);

  caml_release_runtime_system();

  FontDescriptor *query = new FontDescriptor(NULL, NULL, fontFamily, NULL, weightToCheck, widthToCheck, isItalic, isMono);
  FontDescriptor *font = findFont(query);

  delete query;

  caml_acquire_runtime_system();

  if (font == NULL) {
    caml_raise_not_found();
    CAMLreturn(Val_unit);
  }

  ret = caml_alloc(7, 0);

  Store_field(ret, 0, caml_copy_string(font->path));
  Store_field(ret, 1, caml_copy_string(font->postscriptName));
  Store_field(ret, 2, caml_copy_string(font->family));
  Store_field(ret, 3, Val_int(font->weight));
  Store_field(ret, 4, Val_int(font->width));
  Store_field(ret, 5, Val_bool(font->italic));
  Store_field(ret, 6, Val_bool(font->monospace));

  delete font;

  CAMLreturn(ret);
}
}
