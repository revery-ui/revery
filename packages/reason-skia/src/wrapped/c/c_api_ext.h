#pragma once

#include <sk_types.h>

typedef struct sk_svgdom_t sk_svgdom_t;

void sk_svgdom_render(sk_svgdom_t *svgdom, sk_canvas_t *canvas);
void sk_svgdom_set_container_size(sk_svgdom_t *svgdom, float width, float height);
sk_svgdom_t *sk_svgdom_create_from_stream(sk_stream_t *stream);

sk_data_t *sk_data_create_with_cstring(const char *string);
