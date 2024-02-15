#pragma once

#include "defines.h"

typedef union vec2_u {
    Single elements[2];
    struct {
        union {
            Single x, r, s, u;
        };
        union {
            Single y, g, t, v;
        };
    };
} vec2;

typedef struct vec3_u {
    union {
        Single elements[3];
        struct {
            union {
                Single x, r, s, u;
            };
            union {
                Single y, g, t, v;
            };
            union {
                Single z, b, p, w;
            };
        };
    };
} vec3;

typedef struct vec4_u {
#if defined(KUSE_SIMD)
    alignas(16) __m128 data;
#endif
    alignas(16) Single elements[4];
    union {
        struct {
            union {
                Single x, r, s;
            };
            union {
                Single y, g, t;
            };
            union {
                Single z, b, p;
            };
            union {
                Single w, a, q;
            };
        };
    };
} vec4;

typedef vec4 quat;