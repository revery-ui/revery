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

#define UNUSED(x) ((void)(x))

#ifdef USE_COCOA
#define CATEGORY_PROPERTY_GET(type, property) - (type) property { return objc_getAssociatedObject(self, @selector(property)); }
#define CATEGORY_PROPERTY_SET(type, property, setter) - (void) setter (type) property { objc_setAssociatedObject(self, @selector(property), property, OBJC_ASSOCIATION_RETAIN_NONATOMIC); }
#define CATEGORY_PROPERTY_GET_SET(type, property, setter) CATEGORY_PROPERTY_GET(type, property) CATEGORY_PROPERTY_SET(type, property, setter)

#define CATEGORY_PROPERTY_GET_NSNUMBER_PRIMITIVE(type, property, valueSelector) - (type) property { return [objc_getAssociatedObject(self, @selector(property)) valueSelector]; }
#define CATEGORY_PROPERTY_SET_NSNUMBER_PRIMITIVE(type, property, setter, numberSelector) - (void) setter (type) property { objc_setAssociatedObject(self, @selector(property), [NSNumber numberSelector: property], OBJC_ASSOCIATION_RETAIN_NONATOMIC); }

#define CATEGORY_PROPERTY_GET_DOUBLE(property) CATEGORY_PROPERTY_GET_NSNUMBER_PRIMITIVE(double, property, doubleValue)
#define CATEGORY_PROPERTY_SET_DOUBLE(property, setter) CATEGORY_PROPERTY_SET_NSNUMBER_PRIMITIVE(double, property, setter, numberWithDouble)
#define CATEGORY_PROPERTY_GET_SET_DOUBLE(property, setter) CATEGORY_PROPERTY_GET_DOUBLE(property) CATEGORY_PROPERTY_SET_DOUBLE(property, setter)
#endif

