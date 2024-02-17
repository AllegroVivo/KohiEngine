#include <core/logger.h>
#include <math/kmath.h>

#define expect_should_be(expected, actual)                                                              \
    if (actual != expected) {                                                                           \
        KERROR("--> Expected %lld, but got: %lld. File: %s:%d.", expected, actual, __FILE__, __LINE__); \
        return FALSE;                                                                                   \
    }

#define expect_should_not_be(expected, actual)                                                                   \
    if (actual == expected) {                                                                                    \
        KERROR("--> Expected %d != %d, but they are equal. File: %s:%d.", expected, actual, __FILE__, __LINE__); \
        return FALSE;                                                                                            \
    }

#define expect_float_to_be(expected, actual)                                                        \
    if (kabs(expected - actual) > 0.001f) {                                                         \
        KERROR("--> Expected %f, but got: %f. File: %s:%d.", expected, actual, __FILE__, __LINE__); \
        return FALSE;                                                                               \
    }

#define expect_to_be_true(actual)                                                      \
    if (actual != TRUE) {                                                              \
        KERROR("--> Expected TRUE, but got: FALSE. File: %s:%d.", __FILE__, __LINE__); \
        return FALSE;                                                                  \
    }

#define expect_to_be_false(actual)                                                     \
    if (actual != FALSE) {                                                             \
        KERROR("--> Expected FALSE, but got: TRUE. File: %s:%d.", __FILE__, __LINE__); \
        return FALSE;                                                                  \
    }