#include <stdio.h>

#include <caml/mlvalues.h>
#include <caml/bigarray.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/threads.h>
#include <caml/callback.h>

#include <pthread.h>

#include <unistd.h>
#include <string.h>

extern "C" {

    CAMLprim value
    caml_open_sync_raw(value vFileName, value vSuccess, value vFailure) {
        CAMLparam1(vFileName);
        FILE *pFile = fopen("binary.dat", "rb");

        if (!pFile) {
            caml_callback(vFailure, caml_copy_string("Unable to load file"));    
        } else {
            fseek(pFile, 0, SEEK_END);
            long lSize = ftell(pFile);
            fseek(pFile, 0, SEEK_SET);

            char *pData = (char *)malloc(lSize + 1);
            fread(pData, lSize, 1, pFile);

            fclose(pFile);

            long dims[1];
            dims[0] = lSize;
            value ba = caml_ba_alloc(CAML_BA_UINT8 | CAML_BA_C_LAYOUT, 1, pData, (long long int *)dims);
            caml_callback(vSuccess, ba);
        }

        CAMLreturn(Val_unit);
    }

    void *myThreadFun(void *vargp) {
        caml_c_thread_register();
        sleep(5);
        caml_acquire_runtime_system();
        caml_callback(*caml_named_value("test_callback"), caml_copy_string("from c THREAD"));
        caml_release_runtime_system();
        printf("c - from thread\n");
        caml_c_thread_unregister();
        return NULL;
    }

    CAMLprim value
    caml_test_thread() {
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, myThreadFun, NULL);
        return Val_unit;
    }
}
