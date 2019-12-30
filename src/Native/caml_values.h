#ifndef CAMLVALUES_H
#define CAMLVALUES_H

#define Val_none Val_int(0)

static inline value Val_some(value v) {
    (void)Val_some;
    CAMLparam1(v);
    CAMLlocal1(some);
    some = caml_alloc(1, 0);
    Store_field(some, 0, v);
    CAMLreturn(some);
}

#define Some_val(v) Field(v, 0)
#endif