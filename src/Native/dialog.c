#include <stdio.h>

#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/threads.h>

#include "caml_values.h"

#include <string.h>

#include "config.h"
#ifdef USE_WIN32
#include "ReveryWin32.h"
#elif USE_COCOA
#include "ReveryCocoa.h"
#elif USE_GTK
#include "ReveryGtk.h"
#endif
#include "utilities.h"

CAMLprim value revery_alertSupported() {
#if defined(USE_WIN32) || defined(USE_COCOA) || defined(USE_GTK)
    return Val_true;
#else
    return Val_false;
#endif
}

CAMLprim value revery_alert(value vWindow, value vMessage) {
    CAMLparam2(vWindow, vMessage);
    const char *szMessage = String_val(vMessage);
    void *pWin = (void *)revery_unwrapPointer(vWindow);

#ifdef USE_WIN32
    revery_alert_win32(pWin, szMessage);
#elif USE_COCOA
    revery_alert_cocoa(pWin, szMessage);
#elif USE_GTK
    revery_alert_gtk(pWin, szMessage);
#else
    printf("WARNING - Not implemented: alert");
    UNUSED(szMessage);
    UNUSED(pWin);
#endif
    return Val_unit;
}

CAMLprim value revery_alertOpenFiles_native(
    value vStartDirectory, value vFileTypes, value vAllowMultiple,
    value vCanChooseFiles, value vCanChooseDirectories, value vShowHidden,
    value vButtonText, value vTitle, value vUnit) {
    CAMLparam5(vStartDirectory, vFileTypes, vAllowMultiple, vCanChooseFiles,
               vCanChooseDirectories);
    CAMLxparam3(vButtonText, vTitle, vUnit);

    const char *startDirectory;
    const char *buttonText;

    // Initialize an array of filetypes
    const char **fileTypes = NULL;
    int fileTypesSize = 0;

    // title from OCaml -> C
    const char *title;
    if (vTitle != Val_none)
        title = String_val(Some_val(vTitle));
    else
        title = NULL;

    int allowMultiple = Bool_val(vAllowMultiple);
    int canChooseFiles = Bool_val(vCanChooseFiles);
    int canChooseDirectories = Bool_val(vCanChooseDirectories);
    int showHidden = Bool_val(vShowHidden);

    if (vFileTypes != Val_none) {
        CAMLlocal1(camlArr);
        camlArr = Some_val(vFileTypes);
        fileTypesSize = Wosize_val(camlArr);

        // Allocate space for an array
        fileTypes = (const char **)malloc(sizeof(*fileTypes) * fileTypesSize);

        // Populate the array with the CAML array;
        for (int i = 0; i < fileTypesSize; i++) {
            const char *str = String_val(Field(camlArr, i));
            fileTypes[i] = str;
        }
    }

    if (vStartDirectory != Val_none) {
        startDirectory = String_val(Some_val(vStartDirectory));
    } else {
        startDirectory = NULL;
    }

    if (vButtonText != Val_none) {
        buttonText = String_val(Some_val(vButtonText));
    } else {
        buttonText = NULL;
    }

    char **fileList = NULL;

#ifdef USE_WIN32
    fileList = revery_open_files_win32(startDirectory, canChooseFiles,
                                       canChooseDirectories, title);
    (void)fileTypesSize;
    (void)allowMultiple;
    (void)showHidden;
    (void)buttonText;
#elif USE_COCOA
    caml_release_runtime_system();
    fileList = revery_open_files_cocoa(
                   startDirectory, fileTypes, fileTypesSize, allowMultiple, canChooseFiles,
                   canChooseDirectories, showHidden, buttonText, title);
    caml_acquire_runtime_system();
#elif USE_GTK
    fileList = revery_open_files_gtk(
                   startDirectory, fileTypes, fileTypesSize, allowMultiple, canChooseFiles,
                   canChooseDirectories, showHidden, buttonText, title);
#else
    (void)showHidden;
    (void)canChooseDirectories;
    (void)canChooseFiles;
    (void)allowMultiple;
    (void)title;
    (void)buttonText;
    (void)startDirectory;
    (void)fileList;
#endif

    if (fileList) {
        CAMLlocal1(camlArr);
        int len = -1;
        while (fileList[++len] != NULL) {
        }
        camlArr = caml_alloc(len, 0);

        for (int i = 0; i < len; i++) {
            Store_field(camlArr, i, caml_copy_string(fileList[i]));
            free(fileList[i]);
        }

        free(fileList);

        CAMLreturn(Val_some(camlArr));
    } else {
        CAMLreturn(Val_none);
    }
}

CAMLprim value revery_alertOpenFiles_bytecode(value *argv, int argn) {
    (void)argn;
    return revery_alertOpenFiles_native(argv[0], argv[1], argv[2], argv[3],
                                        argv[4], argv[5], argv[6], argv[7],
                                        argv[8]);
}
