#include "kmath.h"
#include "platform/platform.h"

#include <math.h>
#include <stdlib.h>

static Boolean rand_seeded = FALSE;

Single ksin(Single x) {
    return sinf(x);
}

Single kcos(Single x) {
    return cosf(x);
}

Single ktan(Single x) {
    return tanf(x);
}

Single kacos(Single x) {
    return acosf(x);
}

Single ksqrt(Single x) {
    return sqrtf(x);
}

Single kabs(Single x) {
    return fabsf(x);
}

Int32 krandom() {
    if (!rand_seeded) {
        srand((UInt32)platform_get_absolute_time());
        rand_seeded = TRUE;
    }
    return rand();
}

Int32 krandom_in_range(Int32 min, Int32 max) {
    if (!rand_seeded) {
        srand((UInt32)platform_get_absolute_time());
        rand_seeded = TRUE;
    }
    return (rand() % (max - min + 1)) + min;
}

Single fkrandom() {
    return (float)krandom() / (Single)RAND_MAX;
}

Single fkrandom_in_range(Single min, Single max) {
    return min + ((float)krandom() / ((Single)RAND_MAX / (max - min)));
}