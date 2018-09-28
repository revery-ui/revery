#include <stdio.h>

#include <caml/mlvalues.h>
#include <caml/bigarray.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/callback.h>

#include <string.h>

extern "C" {

    CAMLprim value
    caml_open_sync_raw(value vFileName) {
        CAMLparam1(vFileName);


        FILE *pFile = fopen("binary.dat", "rb");

        fseek(pFile, 0, SEEK_END);
        long lSize = ftell(pFile);
        fseek(pFile, 0, SEEK_SET);

        char *pData = (char *)malloc(lSize + 1);
        fread(pData, lSize, 1, pFile);

        fclose(pFile);

        long dims[1];
        dims[0] = lSize;

        return caml_ba_alloc(CAML_BA_UINT8 | CAML_BA_C_LAYOUT, 1, pData, (long long int *)dims);
    }
}
