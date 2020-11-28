#pragma once

/* This was taken from OCaml's config.h.
  It lets us use the value type without
  importing the offending header file
  that causes Obj-C collisions */
#if SIZEOF_PTR == SIZEOF_LONG
/* Standard models: ILP32 or I32LP64 */
typedef long intnat;
typedef unsigned long uintnat;
#define ARCH_INTNAT_PRINTF_FORMAT "l"
#elif SIZEOF_PTR == SIZEOF_INT
/* Hypothetical IP32L64 model */
typedef int intnat;
typedef unsigned int uintnat;
#define ARCH_INTNAT_PRINTF_FORMAT ""
#elif SIZEOF_PTR == 8
/* Win64 model: IL32P64 */
typedef int64_t intnat;
typedef uint64_t uintnat;
#define ARCH_INTNAT_PRINTF_FORMAT ARCH_INT64_PRINTF_FORMAT
#else
#error "No integer type available to represent pointers"
#endif

typedef intnat camlValue;

void revery_caml_call_n(camlValue f, int numArgs, camlValue* args);
void revery_caml_call(camlValue f);
camlValue revery_wrapPointer(void *data);
void *revery_unwrapPointer(camlValue data);
camlValue revery_wrapOptionalPointer(void *data);
