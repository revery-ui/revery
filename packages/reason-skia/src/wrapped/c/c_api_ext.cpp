#include "c_api_ext.h"

void sk_svgdom_unref(sk_svgdom_t *svgdom) {
    SkSafeUnref(AsSVGDOM(svgdom));
}

void sk_svgdom_render(sk_svgdom_t *svgdom, sk_canvas_t *canvas) {
    AsSVGDOM(svgdom)->render(AsCanvas(canvas));
}

void sk_svgdom_set_container_size(sk_svgdom_t *svgdom, float width, float height) {
    AsSVGDOM(svgdom)->setContainerSize(SkSize::Make(width, height));
}
