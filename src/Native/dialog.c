#include <stdio.h>

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/callback.h>

void alert() {
    printf("MESSAGE: %s\n", message);
}
