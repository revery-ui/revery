// C-bindings are needed because the bytecode is generated
// and linked... but aren't actually used! For bytecode/nativecode,
// the native Skia implementation is used.


#define STUB(name)                                                             \
  void name() { return; }
STUB(canvaskit_matrix_make);
STUB(canvaskit_matrix_setIdentity);

STUB(canvaskit_color_float_makeArgb);
STUB(canvaskit_color_float_getA);
STUB(canvaskit_color_float_getR);
STUB(canvaskit_color_float_getG);
STUB(canvaskit_color_float_getB);

STUB(canvaskit_color_int_makeArgb);

STUB(canvaskit_fontManager_makeDefault);

STUB(canvaskit_gr_backendRenderTarget_makeGl);
STUB(canvaskit_gr_gl_interface_makeNative);
STUB(canvaskit_gr_context_makeGl);

STUB(canvaskit_imageInfo_make);

STUB(canvaskit_paint_make);
STUB(canvaskit_paint_setColor);
STUB(canvaskit_paint_setAntiAlias);
STUB(canvaskit_paint_setLcdRenderText);

STUB(canvaskit_rect_makeLtrb);

STUB(canvaskit_rrect_make);

STUB(canvaskit_surface_getCanvas);
STUB(canvaskit_surface_makeRenderTarget);
STUB(canvaskit_surface_makeRenderTarget_byte);
STUB(canvaskit_surface_makeFromBackendRenderTarget);
STUB(canvaskit_surface_makeFromBackendRenderTarget_byte);

STUB(caml_thread_initialize);
STUB(caml_mutex_new);

STUB(revery_getUserLocale);

STUB(timber_prerr_native);
