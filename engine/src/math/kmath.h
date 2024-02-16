#pragma once

#include "defines.h"
#include "math_types.h"
#include "core/kmemory.h"

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

KINLINE Single vec4_dot_Single(
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

KINLINE mat4 mat4_identity() {
    mat4 out_matrix;
    kzero_memory(out_matrix.data, sizeof(Single) * 16);
    out_matrix.data[0] = 1.f;
    out_matrix.data[5] = 1.f;
    out_matrix.data[10] = 1.f;
    out_matrix.data[15] = 1.f;
    return out_matrix;
}

KINLINE mat4 mat4_mul(mat4 matrix_0, mat4 matrix_1) {
    mat4 out_matrix = mat4_identity();

    const Single* m1_ptr = matrix_0.data;
    const Single* m2_ptr = matrix_1.data;
    Single* dst_ptr = out_matrix.data;

    for (Int32 i = 0; i < 4; ++i) {
        for (Int32 j = 0; j < 4; j++) {
            *dst_ptr = 
                m1_ptr[0] * m2_ptr[0 + j] +
                m1_ptr[1] * m2_ptr[4 + j] +
                m1_ptr[2] * m2_ptr[8 + j] +
                m1_ptr[3] * m2_ptr[12 + j];
            dst_ptr++;
        }
        
        m1_ptr += 4;
    }

    return out_matrix;
}

KINLINE mat4 mat4_orthographic(Single left, Single right, Single bottom, Single top, Single near_clip, Single far_clip) {
    mat4 out_matrix = mat4_identity();

    Single lr = 1.f / (left - right);
    Single bt = 1.f / (bottom - top);
    Single nf = 1.f / (near_clip - far_clip);

    out_matrix.data[0] = -2.f * lr;
    out_matrix.data[5] = -2.f * bt;
    out_matrix.data[10] = 2.f * nf;

    out_matrix.data[12] = (left + right) * lr;
    out_matrix.data[13] = (top + bottom) * bt;
    out_matrix.data[14] = (far_clip + near_clip) * nf;

    return out_matrix;
}

KINLINE mat4 mat4_perspective(Single fov_radians, Single aspect_ratio, Single near_clip, Single far_clip) {
    Single half_tan_fov = ktan(fov_radians * 0.5f);
    mat4 out_matrix;
    kzero_memory(out_matrix.data, sizeof(Single) * 16);
    out_matrix.data[0] = 1.f / (aspect_ratio * half_tan_fov);
    out_matrix.data[5] = 1.f / half_tan_fov;
    out_matrix.data[10] = -((far_clip + near_clip) / (far_clip - near_clip));
    out_matrix.data[11] = -1.f;
    out_matrix.data[14] = -((2.f * far_clip * near_clip) / (far_clip - near_clip));
    return out_matrix;
}

KINLINE mat4 mat4_look_at(vec3 position, vec3 target, vec3 up) {
    mat4 out_matrix;
    vec3 z_axis;
    z_axis.x = target.x - position.x;
    z_axis.y = target.y - position.y;
    z_axis.z = target.z - position.z;

    z_axis = vec3_normalized(z_axis);
    vec3 x_axis = vec3_normalized(vec3_cross(z_axis, up));
    vec3 y_axis = vec3_cross(x_axis, z_axis);

    out_matrix.data[0] = x_axis.x;
    out_matrix.data[1] = y_axis.x;
    out_matrix.data[2] = -z_axis.x;
    out_matrix.data[3] = 0;
    out_matrix.data[4] = x_axis.y;
    out_matrix.data[5] = y_axis.y;
    out_matrix.data[6] = -z_axis.y;
    out_matrix.data[7] = 0;
    out_matrix.data[8] = x_axis.z;
    out_matrix.data[9] = y_axis.z;
    out_matrix.data[10] = -z_axis.z;
    out_matrix.data[11] = 0;
    out_matrix.data[12] = -vec3_dot(x_axis, position);
    out_matrix.data[13] = -vec3_dot(y_axis, position);
    out_matrix.data[14] =  vec3_dot(z_axis, position);
    out_matrix.data[15] = 1.f;

    return out_matrix;
}

KINLINE mat4 mat4_inverse(mat4 matrix) {
    const Single* m = matrix.data;

    Single t0 = m[10] * m[15];
    Single t1 = m[14] * m[11];
    Single t2 = m[6] * m[15];
    Single t3 = m[14] * m[7];
    Single t4 = m[6] * m[11];
    Single t5 = m[10] * m[7];
    Single t6 = m[2] * m[15];
    Single t7 = m[14] * m[3];
    Single t8 = m[2] * m[11];
    Single t9 = m[10] * m[3];
    Single t10 = m[2] * m[7];
    Single t11 = m[6] * m[3];
    Single t12 = m[8] * m[13];
    Single t13 = m[12] * m[9];
    Single t14 = m[4] * m[13];
    Single t15 = m[12] * m[5];
    Single t16 = m[4] * m[9];
    Single t17 = m[8] * m[5];
    Single t18 = m[0] * m[13];
    Single t19 = m[12] * m[1];
    Single t20 = m[0] * m[9];
    Single t21 = m[8] * m[1];
    Single t22 = m[0] * m[5];
    Single t23 = m[4] * m[1];

    mat4 out_matrix;
    Single* o = out_matrix.data;

    o[0] = (t0 * m[5] + t3 * m[9] + t4 * m[13]) - (t1 * m[5] + t2 * m[9] + t5 * m[13]);
    o[1] = (t1 * m[1] + t6 * m[9] + t9 * m[13]) - (t0 * m[1] + t7 * m[9] + t8 * m[13]);
    o[2] = (t2 * m[1] + t7 * m[5] + t10 * m[13]) - (t3 * m[1] + t6 * m[5] + t11 * m[13]);
    o[3] = (t5 * m[1] + t8 * m[5] + t11 * m[9]) - (t4 * m[1] + t9 * m[5] + t10 * m[9]);

    Single d = 1.0f / (m[0] * o[0] + m[4] * o[1] + m[8] * o[2] + m[12] * o[3]);

    o[0] = d * o[0];
    o[1] = d * o[1];
    o[2] = d * o[2];
    o[3] = d * o[3];
    o[4] = d * ((t1 * m[4] + t2 * m[8] + t5 * m[12]) - (t0 * m[4] + t3 * m[8] + t4 * m[12]));
    o[5] = d * ((t0 * m[0] + t7 * m[8] + t8 * m[12]) - (t1 * m[0] + t6 * m[8] + t9 * m[12]));
    o[6] = d * ((t3 * m[0] + t6 * m[4] + t11 * m[12]) - (t2 * m[0] + t7 * m[4] + t10 * m[12]));
    o[7] = d * ((t4 * m[0] + t9 * m[4] + t10 * m[8]) - (t5 * m[0] + t8 * m[4] + t11 * m[8]));
    o[8] = d * ((t12 * m[7] + t15 * m[11] + t16 * m[15]) - (t13 * m[7] + t14 * m[11] + t17 * m[15]));
    o[9] = d * ((t13 * m[3] + t18 * m[11] + t21 * m[15]) - (t12 * m[3] + t19 * m[11] + t20 * m[15]));
    o[10] = d * ((t14 * m[3] + t19 * m[7] + t22 * m[15]) - (t15 * m[3] + t18 * m[7] + t23 * m[15]));
    o[11] = d * ((t17 * m[3] + t20 * m[7] + t23 * m[11]) - (t16 * m[3] + t21 * m[7] + t22 * m[11]));
    o[12] = d * ((t14 * m[10] + t17 * m[14] + t13 * m[6]) - (t16 * m[14] + t12 * m[6] + t15 * m[10]));
    o[13] = d * ((t20 * m[14] + t12 * m[2] + t19 * m[10]) - (t18 * m[10] + t21 * m[14] + t13 * m[2]));
    o[14] = d * ((t18 * m[6] + t23 * m[14] + t15 * m[2]) - (t22 * m[14] + t14 * m[2] + t19 * m[6]));
    o[15] = d * ((t22 * m[10] + t16 * m[2] + t21 * m[6]) - (t20 * m[6] + t23 * m[10] + t17 * m[2]));

    return out_matrix;
}

KINLINE mat4 mat4_translation(vec3 position) {
    mat4 out_matrix = mat4_identity();
    out_matrix.data[12] = position.x;
    out_matrix.data[13] = position.y;
    out_matrix.data[14] = position.z;
    return out_matrix;
}

KINLINE mat4 mat4_scale(vec3 scale) {
    mat4 out_matrix = mat4_identity();
    out_matrix.data[0] = scale.x;
    out_matrix.data[5] = scale.y;
    out_matrix.data[10] = scale.z;
    return out_matrix;
}

KINLINE mat4 mat4_euler_x(Single angle_radians) {
    mat4 out_matrix = mat4_identity();
    Single c = kcos(angle_radians);
    Single s = ksin(angle_radians);

    out_matrix.data[5] = c;
    out_matrix.data[6] = s;
    out_matrix.data[9] = -s;
    out_matrix.data[10] = c;
    return out_matrix;
}
KINLINE mat4 mat4_euler_y(Single angle_radians) {
    mat4 out_matrix = mat4_identity();
    Single c = kcos(angle_radians);
    Single s = ksin(angle_radians);

    out_matrix.data[0] = c;
    out_matrix.data[2] = -s;
    out_matrix.data[8] = s;
    out_matrix.data[10] = c;
    return out_matrix;
}
KINLINE mat4 mat4_euler_z(Single angle_radians) {
    mat4 out_matrix = mat4_identity();

    Single c = kcos(angle_radians);
    Single s = ksin(angle_radians);

    out_matrix.data[0] = c;
    out_matrix.data[1] = s;
    out_matrix.data[4] = -s;
    out_matrix.data[5] = c;
    return out_matrix;
}
KINLINE mat4 mat4_euler_xyz(Single x_radians, Single y_radians, Single z_radians) {
    mat4 rx = mat4_euler_x(x_radians);
    mat4 ry = mat4_euler_y(y_radians);
    mat4 rz = mat4_euler_z(z_radians);
    mat4 out_matrix = mat4_mul(rx, ry);
    out_matrix = mat4_mul(out_matrix, rz);
    return out_matrix;
}

KINLINE vec3 mat4_forward(mat4 matrix) {
    vec3 forward;
    forward.x = -matrix.data[2];
    forward.y = -matrix.data[6];
    forward.z = -matrix.data[10];
    vec3_normalize(&forward);
    return forward;
}

KINLINE vec3 mat4_backward(mat4 matrix) {
    vec3 backward;
    backward.x = matrix.data[2];
    backward.y = matrix.data[6];
    backward.z = matrix.data[10];
    vec3_normalize(&backward);
    return backward;
}

KINLINE vec3 mat4_up(mat4 matrix) {
    vec3 up;
    up.x = matrix.data[1];
    up.y = matrix.data[5];
    up.z = matrix.data[9];
    vec3_normalize(&up);
    return up;
}

KINLINE vec3 mat4_down(mat4 matrix) {
    vec3 down;
    down.x = -matrix.data[1];
    down.y = -matrix.data[5];
    down.z = -matrix.data[9];
    vec3_normalize(&down);
    return down;
}

KINLINE vec3 mat4_left(mat4 matrix) {
    vec3 right;
    right.x = -matrix.data[0];
    right.y = -matrix.data[4];
    right.z = -matrix.data[8];
    vec3_normalize(&right);
    return right;
}

KINLINE vec3 mat4_right(mat4 matrix) {
    vec3 left;
    left.x = matrix.data[0];
    left.y = matrix.data[4];
    left.z = matrix.data[8];
    vec3_normalize(&left);
    return left;
}

// ------------------------------------------
// Quaternion
// ------------------------------------------

KINLINE quat quat_identity() {
    return (quat){0, 0, 0, 1.0f};
}

KINLINE Single quat_normal(quat q) {
    return ksqrt(
        q.x * q.x +
        q.y * q.y +
        q.z * q.z +
        q.w * q.w);
}

KINLINE quat quat_normalize(quat q) {
    Single normal = quat_normal(q);
    return (quat){
        q.x / normal,
        q.y / normal,
        q.z / normal,
        q.w / normal};
}

KINLINE quat quat_conjugate(quat q) {
    return (quat){
        -q.x,
        -q.y,
        -q.z,
        q.w};
}

KINLINE quat quat_inverse(quat q) {
    return quat_normalize(quat_conjugate(q));
}

KINLINE quat quat_mul(quat q_0, quat q_1) {
    quat out_quaternion;

    out_quaternion.x = q_0.x * q_1.w +
                       q_0.y * q_1.z -
                       q_0.z * q_1.y +
                       q_0.w * q_1.x;

    out_quaternion.y = -q_0.x * q_1.z +
                       q_0.y * q_1.w +
                       q_0.z * q_1.x +
                       q_0.w * q_1.y;

    out_quaternion.z = q_0.x * q_1.y -
                       q_0.y * q_1.x +
                       q_0.z * q_1.w +
                       q_0.w * q_1.z;

    out_quaternion.w = -q_0.x * q_1.x -
                       q_0.y * q_1.y -
                       q_0.z * q_1.z +
                       q_0.w * q_1.w;

    return out_quaternion;
}

KINLINE Single quat_dot(quat q_0, quat q_1) {
    return q_0.x * q_1.x +
           q_0.y * q_1.y +
           q_0.z * q_1.z +
           q_0.w * q_1.w;
}

KINLINE mat4 quat_to_mat4(quat q) {
    mat4 out_matrix = mat4_identity();

    // https://stackoverflow.com/questions/1556260/convert-quaternion-rotation-to-rotation-matrix

    quat n = quat_normalize(q);

    out_matrix.data[0] = 1.0f - 2.0f * n.y * n.y - 2.0f * n.z * n.z;
    out_matrix.data[1] = 2.0f * n.x * n.y - 2.0f * n.z * n.w;
    out_matrix.data[2] = 2.0f * n.x * n.z + 2.0f * n.y * n.w;

    out_matrix.data[4] = 2.0f * n.x * n.y + 2.0f * n.z * n.w;
    out_matrix.data[5] = 1.0f - 2.0f * n.x * n.x - 2.0f * n.z * n.z;
    out_matrix.data[6] = 2.0f * n.y * n.z - 2.0f * n.x * n.w;

    out_matrix.data[8] = 2.0f * n.x * n.z - 2.0f * n.y * n.w;
    out_matrix.data[9] = 2.0f * n.y * n.z + 2.0f * n.x * n.w;
    out_matrix.data[10] = 1.0f - 2.0f * n.x * n.x - 2.0f * n.y * n.y;

    return out_matrix;
}

KINLINE mat4 quat_to_rotation_matrix(quat q, vec3 center) {
    mat4 out_matrix;

    Single* o = out_matrix.data;
    o[0] = (q.x * q.x) - (q.y * q.y) - (q.z * q.z) + (q.w * q.w);
    o[1] = 2.0f * ((q.x * q.y) + (q.z * q.w));
    o[2] = 2.0f * ((q.x * q.z) - (q.y * q.w));
    o[3] = center.x - center.x * o[0] - center.y * o[1] - center.z * o[2];

    o[4] = 2.0f * ((q.x * q.y) - (q.z * q.w));
    o[5] = -(q.x * q.x) + (q.y * q.y) - (q.z * q.z) + (q.w * q.w);
    o[6] = 2.0f * ((q.y * q.z) + (q.x * q.w));
    o[7] = center.y - center.x * o[4] - center.y * o[5] - center.z * o[6];

    o[8] = 2.0f * ((q.x * q.z) + (q.y * q.w));
    o[9] = 2.0f * ((q.y * q.z) - (q.x * q.w));
    o[10] = -(q.x * q.x) - (q.y * q.y) + (q.z * q.z) + (q.w * q.w);
    o[11] = center.z - center.x * o[8] - center.y * o[9] - center.z * o[10];

    o[12] = 0.0f;
    o[13] = 0.0f;
    o[14] = 0.0f;
    o[15] = 1.0f;
    return out_matrix;
}

KINLINE quat quat_from_axis_angle(vec3 axis, Single angle, Boolean normalize) {
    const Single half_angle = 0.5f * angle;
    Single s = ksin(half_angle);
    Single c = kcos(half_angle);

    quat q = (quat){s * axis.x, s * axis.y, s * axis.z, c};
    if (normalize) {
        return quat_normalize(q);
    }
    return q;
}

KINLINE quat quat_slerp(quat q_0, quat q_1, Single percentage) {
    quat out_quaternion;
    // Source: https://en.wikipedia.org/wiki/Slerp
    quat v0 = quat_normalize(q_0);
    quat v1 = quat_normalize(q_1);

    Single dot = quat_dot(v0, v1);

    if (dot < 0.0f) {
        v1.x = -v1.x;
        v1.y = -v1.y;
        v1.z = -v1.z;
        v1.w = -v1.w;
        dot = -dot;
    }

    const Single DOT_THRESHOLD = 0.9995f;
    if (dot > DOT_THRESHOLD) {
        out_quaternion = (quat){
            v0.x + ((v1.x - v0.x) * percentage),
            v0.y + ((v1.y - v0.y) * percentage),
            v0.z + ((v1.z - v0.z) * percentage),
            v0.w + ((v1.w - v0.w) * percentage)};

        return quat_normalize(out_quaternion);
    }

    Single theta_0 = kacos(dot);
    Single theta = theta_0 * percentage;
    Single sin_theta = ksin(theta);
    Single sin_theta_0 = ksin(theta_0);

    Single s0 = kcos(theta) - dot * sin_theta / sin_theta_0;
    Single s1 = sin_theta / sin_theta_0;

    return (quat){
        (v0.x * s0) + (v1.x * s1),
        (v0.y * s0) + (v1.y * s1),
        (v0.z * s0) + (v1.z * s1),
        (v0.w * s0) + (v1.w * s1)};
}

KINLINE Single deg_to_rad(Single degrees) {
    return degrees * K_DEG2RAD_MULTIPLIER;
}

KINLINE Single rad_to_deg(Single radians) {
    return radians * K_RAD2DEG_MULTIPLIER;
}