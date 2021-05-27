
// Provides: canvaskit_matrix_make
function canvaskit_matrix_make() {
    return CanvasKit.Matrix.identity();
}

// Provides: canvaskit_matrix_setIdentity
function canvaskit_matrix_setIdentity(m) {
    m[0] = 1;
    m[1] = 0;
    m[2] = 0;

    m[3] = 0;
    m[4] = 1;
    m[5] = 0;

    m[6] = 0;
    m[7] = 0;
    m[8] = 0;
}

// Provides: canvaskit_color_float_makeArgb
function canvaskit_color_float_makeArgb(a, r, g, b) {
    return CanvasKit.Color4f(a, r, g, b);
};

// Provides: canvaskit_rect_makeLtrb
function canvaskit_rect_makeLtrb(left, top, right, bottom) {
    return CanvasKit.LTRBRect(left, top, right, bottom);
}

// Provides: canvaskit_color_int_makeArgb
function canvaskit_color_int_makeArgb(a, r, g, b) {
    return CanvasKit.Color(a, r, g, b);
};

// Provides: canvaskit_paint_setColor
function canvaskit_paint_setColor(paint, color) {
    paint.setColor(color);
}

// Provides: canvaskit_paint_setAntiAlias
function canvaskit_paint_setAntiAlias(paint, antiAlias) {
    paint.setAntiAlias(antiAlias);
}

// Provides: canvaskit_color_float_getA
function canvaskit_color_float_getA(color) {
    return color[0]
};

// Provides: canvaskit_color_float_getR
function canvaskit_color_float_getR(color) {
    return color[1]
};

// Provides: canvaskit_color_float_getG
function canvaskit_color_float_getG(color) {
    return color[2]
};

// Provides: canvaskit_color_float_getB
function canvaskit_color_float_getB(color) {
    return color[3]
};

// Provides: canvaskit_fontManager_makeDefault
function canvaskit_fontManager_makeDefault() {
    return null;
}

// Provides: canvaskit_paint_make
function canvaskit_paint_make() {
    return new CanvasKit.Paint();
}

// Provides: canvaskit_paint_setLcdRenderText
function canvaskit_paint_setLcdRenderText() {
    // Noop in browser
    return;
}

// Provides: canvaskit_rrect_make
function canvaskit_rrect_make() {
    return CanvasKit.RRectXY(
        CanvasKit.LTRBRect(1, 1, 2, 2),
        1,
        1
    );
}

// Provides: caml_thread_initialize
function caml_thread_initialize() {
    // noop
}

// Provides: caml_mutex_new
function caml_mutex_new() {
    // noop
}

// Provides: caml_mutex_lock
function caml_mutex_lock() {
    // noop
}

// Provides: caml_mutex_unlock
function caml_mutex_unlock() {
    // noop
}

// Provides: revery_getUserLocale
// Requires: caml_js_to_string
function revery_getUserLocale() {
    // TODO?
    return caml_js_to_string("en-us")
}

// Provides: timber_prerr_native
// Requires: caml_js_from_string
function timber_prerr_native(str) {
    console.log(caml_js_from_string(str));
}

// Provides: revery_initializeWindow
function revery_initializeWindow(win) {
    return;
}

// Provides: revery_initializeApp
function revery_initializeApp(app) {
    return;
}
