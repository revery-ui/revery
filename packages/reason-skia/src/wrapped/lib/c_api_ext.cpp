#include <model/SkSVGDOM.h>
#include <SkCanvas.h>
#include <SkSize.h>
#include <SkStream.h>
#include <SkData.h>

#include "c_api_ext.h"
#include "c_api_ext_utils.h"

extern "C" {
    // SVGDOM
    void sk_svgdom_unref(sk_svgdom_t *svgdom) {
        SkSafeUnref(AsSVGDOM(svgdom));
    }

    void sk_svgdom_render(sk_svgdom_t *svgdom, sk_canvas_t *canvas) {
        AsSVGDOM(svgdom)->render(AsCanvas(canvas));
    }

    void sk_svgdom_set_container_size(sk_svgdom_t *svgdom, float width, float height) {
        AsSVGDOM(svgdom)->setContainerSize(SkSize::Make(width, height));
    }

    float sk_svgdom_get_container_width(sk_svgdom_t *svgdom) {
        return AsSVGDOM(svgdom)->containerSize().width();
    }

    float sk_svgdom_get_container_height(sk_svgdom_t *svgdom) {
        return AsSVGDOM(svgdom)->containerSize().height();
    }

    sk_svgdom_t *sk_svgdom_create_from_stream(sk_stream_t *stream) {
        std::unique_ptr<SkStream> skstream(AsStream(stream));
        return ToSVGDOM(SkSVGDOM::MakeFromStream(*skstream).release());
    }

    // Data
    sk_data_t *sk_data_create_with_cstring(const char *string) {
        return ToData(SkData::MakeWithCString(string).release());
    }
}
