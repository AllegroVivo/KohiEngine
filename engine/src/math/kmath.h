#pragma once

#include "defines.h"
#include "math_types.h"

#define K_PI 3.14159265358979323846f
#define K_PI_2 2.0f * K_PI
#define K_HALF_PI 0.5f * K_PI
#define K_QUARTER_PI 0.25f * K_PI
#define K_ONE_OVER_PI 1.0f / K_PI
#define K_ONE_OVER_TWO_PI 1.0f / K_PI_2
#define K_SQRT_TWO 1.41421356237309504880f
#define K_SQRT_THREE 1.73205080756887729352f
#define K_SQRT_ONE_OVER_TWO 0.70710678118654752440f
#define K_SQRT_ONE_OVER_THREE 0.57735026918962576450f
#define K_DEG2RAD_MULTIPLIER K_PI / 180.0f
#define K_RAD2DEG_MULTIPLIER 180.0f / K_PI

// The multiplier to convert seconds to milliseconds.
#define K_SEC_TO_MS_MULTIPLIER 1000.0f

// The multiplier to convert milliseconds to seconds.
#define K_MS_TO_SEC_MULTIPLIER 0.001f

// A huge number that should be larger than any valid number used.
#define K_INFINITY 1e30f

// Smallest positive number where 1.0 + FLOAT_EPSILON != 0
#define K_FLOAT_EPSILON 1.192092896e-07f

// ------------------------------------------
// General math functions
// ------------------------------------------
KAPI Single ksin(Single x);
KAPI Single kcos(Single x);
KAPI Single ktan(Single x);
KAPI Single kacos(Single x);
KAPI Single ksqrt(Single x);
KAPI Single kabs(Single x);

KINLINE Boolean is_power_of_2(UInt64 value) {
    return (value != 0) && ((value & (value - 1)) == 0);
}

KAPI Int32 krandom();
KAPI Int32 krandom_in_range(Int32 min, Int32 max);

KAPI Single fkrandom();
KAPI Single fkrandom_in_range(Single min, Single max);

// ------------------------------------------
// Vector 2
// ------------------------------------------
KINLINE vec2 vec2_create(Single x, Single y) {
    vec2 out_vector;
    out_vector.x = x;
    out_vector.y = y;
    return out_vector;
}

KINLINE vec2 vec2_zero() {
    return (vec2){ 0.f, 0.f };
}

KINLINE vec2 vec2_one() {
    return (vec2){ 1.f, 1.f };
}

KINLINE vec2 vec2_up() {
    return (vec2){ 0.f, 1.f };
}

KINLINE vec2 vec2_down() {
    return (vec2){ 0.f, -1.f };
}

KINLINE vec2 vec2_left1() {
    return (vec2){ -1.f, 0.f };
}

KINLINE vec2 vec2_right() {
    return (vec2){ 1.f, 0.f };
}

KINLINE vec2 vec2_add(vec2 vector_0, vec2 vector_1) {
    return (vec2){
        vector_0.x + vector_1.x,
        vector_0.y + vector_1.y};
}

KINLINE vec2 vec2_sub(vec2 vector_0, vec2 vector_1) {
    return (vec2){
        vector_0.x - vector_1.x,
        vector_0.y - vector_1.y};
}

KINLINE vec2 vec2_mul(vec2 vector_0, vec2 vector_1) {
    return (vec2){
        vector_0.x * vector_1.x,
        vector_0.y * vector_1.y};
}

KINLINE vec2 vec2_div(vec2 vector_0, vec2 vector_1) {
    return (vec2){
        vector_0.x / vector_1.x,
        vector_0.y / vector_1.y};
}

KINLINE Single vec2_length_squared(vec2 vector) {
    return vector.x * vector.x + vector.y * vector.y;
}

KINLINE Single vec2_length(vec2 vector) {
    return ksqrt(vec2_length_squared(vector));
}

KINLINE void vec2_normalize(vec2* vector) {
    const Single length = vec2_length(*vector);
    vector->x /= length;
    vector->y /= length;
}

KINLINE vec2 vec2_normalized(vec2 vector) { 
    vec2_normalize(&vector);
    return vector;
}

KINLINE Boolean vec2_compare(vec2 vector_0, vec2 vector_1, Single tolerance) {
    if (kabs(vector_0.x - vector_1.x) > tolerance) {
        return FALSE;
    }

    if (kabs(vector_0.y - vector_1.y) > tolerance) {
        return FALSE;
    }

    return TRUE;
}

KINLINE Single vec2_distance(vec2 vector_0, vec2 vector_1) {
    vec2 d = (vec2){
        vector_0.x - vector_1.x,
        vector_0.y - vector_1.y};
    return vec2_length(d);
}

// ------------------------------------------
// Vector 3
// ------------------------------------------

KINLINE vec3 vec3_create(Single x, Single y, Single z) {
    return (vec3){x, y, z};
}

KINLINE vec3 vec3_from_vec4(vec4 vector) {
    return (vec3){vector.x, vector.y, vector.z};
}

KINLINE vec4 vec3_to_vec4(vec3 vector, Single w) {
    return (vec4){vector.x, vector.y, vector.z, w};
}

KINLINE vec3 vec3_zero() {
    return (vec3){0.0f, 0.0f, 0.0f};
}

KINLINE vec3 vec3_one() {
    return (vec3){1.0f, 1.0f, 1.0f};
}

KINLINE vec3 vec3_up() {
    return (vec3){0.0f, 1.0f, 0.0f};
}

KINLINE vec3 vec3_down() {
    return (vec3){0.0f, -1.0f, 0.0f};
}

KINLINE vec3 vec3_left() {
    return (vec3){-1.0f, 0.0f, 0.0f};
}

KINLINE vec3 vec3_right() {
    return (vec3){1.0f, 0.0f, 0.0f};
}

KINLINE vec3 vec3_forward() {
    return (vec3){0.0f, 0.0f, -1.0f};
}

KINLINE vec3 vec3_back() {
    return (vec3){0.0f, 0.0f, 1.0f};
}

KINLINE vec3 vec3_add(vec3 vector_0, vec3 vector_1) {
    return (vec3){
        vector_0.x + vector_1.x,
        vector_0.y + vector_1.y,
        vector_0.z + vector_1.z};
}

KINLINE vec3 vec3_sub(vec3 vector_0, vec3 vector_1) {
    return (vec3){
        vector_0.x - vector_1.x,
        vector_0.y - vector_1.y,
        vector_0.z - vector_1.z};
}

KINLINE vec3 vec3_mul(vec3 vector_0, vec3 vector_1) {
    return (vec3){
        vector_0.x * vector_1.x,
        vector_0.y * vector_1.y,
        vector_0.z * vector_1.z};
}

KINLINE vec3 vec3_mul_scalar(vec3 vector_0, Single scalar) {
    return (vec3){
        vector_0.x * scalar,
        vector_0.y * scalar,
        vector_0.z * scalar};
}

KINLINE vec3 vec3_div(vec3 vector_0, vec3 vector_1) {
    return (vec3){
        vector_0.x / vector_1.x,
        vector_0.y / vector_1.y,
        vector_0.z / vector_1.z};
}

KINLINE Single vec3_length_squared(vec3 vector) {
    return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}

KINLINE Single vec3_length(vec3 vector) {
    return ksqrt(vec3_length_squared(vector));
}

KINLINE void vec3_normalize(vec3* vector) {
    const Single length = vec3_length(*vector);
    vector->x /= length;
    vector->y /= length;
    vector->z /= length;
}

KINLINE vec3 vec3_normalized(vec3 vector) {
    vec3_normalize(&vector);
    return vector;
}

KINLINE Single vec3_dot(vec3 vector_0, vec3 vector_1) {
    Single p = 0;
    p += vector_0.x * vector_1.x;
    p += vector_0.y * vector_1.y;
    p += vector_0.z * vector_1.z;
    return p;
}

KINLINE vec3 vec3_cross(vec3 vector_0, vec3 vector_1) {
    return (vec3){
        vector_0.y * vector_1.z - vector_0.z * vector_1.y,
        vector_0.z * vector_1.x - vector_0.x * vector_1.z,
        vector_0.x * vector_1.y - vector_0.y * vector_1.x};
}

KINLINE const Boolean vec3_compare(vec3 vector_0, vec3 vector_1, Single tolerance) {
    if (kabs(vector_0.x - vector_1.x) > tolerance) {
        return FALSE;
    }

    if (kabs(vector_0.y - vector_1.y) > tolerance) {
        return FALSE;
    }

    if (kabs(vector_0.z - vector_1.z) > tolerance) {
        return FALSE;
    }

    return TRUE;
}

KINLINE Single vec3_distance(vec3 vector_0, vec3 vector_1) {
    vec3 d = (vec3){
        vector_0.x - vector_1.x,
        vector_0.y - vector_1.y,
        vector_0.z - vector_1.z};
    return vec3_length(d);
}

// ------------------------------------------
// Vector 4
// ------------------------------------------

KINLINE vec4 vec4_create(Single x, Single y, Single z, Single w) {
    vec4 out_vector;
#if defined(KUSE_SIMD)
    out_vector.data = _mm_setr_ps(x, y, z, w);
#else
    out_vector.x = x;
    out_vector.y = y;
    out_vector.z = z;
    out_vector.w = w;
#endif
    return out_vector;
}

KINLINE vec3 vec4_to_vec3(vec4 vector) {
    return (vec3){vector.x, vector.y, vector.z};
}

KINLINE vec4 vec4_from_vec3(vec3 vector, Single w) {
#if defined(KUSE_SIMD)
    vec4 out_vector;
    out_vector.data = _mm_setr_ps(x, y, z, w);
    return out_vector;
#else
    return (vec4){vector.x, vector.y, vector.z, w};
#endif
}

KINLINE vec4 vec4_zero() {
    return (vec4){0.0f, 0.0f, 0.0f, 0.0f};
}

KINLINE vec4 vec4_one() {
    return (vec4){1.0f, 1.0f, 1.0f, 1.0f};
}

KINLINE vec4 vec4_add(vec4 vector_0, vec4 vector_1) {
    vec4 result;
     for (UInt64 i = 0; i < 4; ++i) {
        result.elements[i] = vector_0.elements[i] + vector_1.elements[i];
    }
    return result;
}

KINLINE vec4 vec4_sub(vec4 vector_0, vec4 vector_1) {
    vec4 result;
    for (UInt64 i = 0; i < 4; ++i) {
        result.elements[i] = vector_0.elements[i] - vector_1.elements[i];
    }
    return result;
}

KINLINE vec4 vec4_mul(vec4 vector_0, vec4 vector_1) {
    vec4 result;
    for (UInt64 i = 0; i < 4; ++i) {
        result.elements[i] = vector_0.elements[i] * vector_1.elements[i];
    }
    return result;
}

KINLINE vec4 vec4_div(vec4 vector_0, vec4 vector_1) {
    vec4 result;
    for (UInt64 i = 0; i < 4; ++i) {
        result.elements[i] = vector_0.elements[i] / vector_1.elements[i];
    }
    return result;
}

KINLINE Single vec4_length_squared(vec4 vector) {
    return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w;
}

KINLINE Single vec4_length(vec4 vector) {
    return ksqrt(vec4_length_squared(vector));
}

KINLINE void vec4_normalize(vec4* vector) {
    const Single length = vec4_length(*vector);
    vector->x /= length;
    vector->y /= length;
    vector->z /= length;
    vector->w /= length;
}

KINLINE vec4 vec4_normalized(vec4 vector) {
    vec4_normalize(&vector);
    return vector;
}

KINLINE Single vec4_dot_f32(
    Single a0, Single a1, Single a2, Single a3,
    Single b0, Single b1, Single b2, Single b3) {
    Single p;
    p =
        a0 * b0 +
        a1 * b1 +
        a2 * b2 +
        a3 * b3;
    return p;
}