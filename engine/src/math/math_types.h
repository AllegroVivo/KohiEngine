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
    Single elements[4];
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

typedef union mat4_u {
    Single data[16];
} mat4;

