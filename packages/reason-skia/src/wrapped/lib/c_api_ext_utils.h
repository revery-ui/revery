#pragma once

#include <sk_types.h>

#define DEF_MAP_DECL(SkType, sk_type, Name, Declaration)       \
    Declaration;                                               \
    static inline const SkType& As##Name(const sk_type& t) {   \
        return reinterpret_cast<const SkType&>(t);             \
    }                                                          \
    static inline const SkType* As##Name(const sk_type* t) {   \
        return reinterpret_cast<const SkType*>(t);             \
    }                                                          \
    static inline SkType& As##Name(sk_type& t) {               \
        return reinterpret_cast<SkType&>(t);                   \
    }                                                          \
    static inline SkType* As##Name(sk_type* t) {               \
        return reinterpret_cast<SkType*>(t);                   \
    }                                                          \
    static inline const sk_type& To##Name(const SkType& t) {   \
        return reinterpret_cast<const sk_type&>(t);            \
    }                                                          \
    static inline const sk_type* To##Name(const SkType* t) {   \
        return reinterpret_cast<const sk_type*>(t);            \
    }                                                          \
    static inline sk_type& To##Name(SkType& t) {               \
        return reinterpret_cast<sk_type&>(t);                  \
    }                                                          \
    static inline sk_type* To##Name(SkType* t) {               \
        return reinterpret_cast<sk_type*>(t);                  \
    }

#define DEF_CLASS_MAP(SkType, sk_type, Name)                   \
    DEF_MAP_DECL(SkType, sk_type, Name, class SkType)

#define DEF_STRUCT_MAP(SkType, sk_type, Name)                  \
    DEF_MAP_DECL(SkType, sk_type, Name, struct SkType)

#define DEF_MAP(SkType, sk_type, Name)                         \
    DEF_MAP_DECL(SkType, sk_type, Name, )


DEF_CLASS_MAP(SkSVGDOM, sk_svgdom_t, SVGDOM)
DEF_CLASS_MAP(SkCanvas, sk_canvas_t, Canvas)
DEF_CLASS_MAP(SkStream, sk_stream_t, Stream)
DEF_CLASS_MAP(SkData, sk_data_t, Data)
