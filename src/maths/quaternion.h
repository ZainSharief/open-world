#ifndef QUATERNION_H
#define QUATERNION_H

#include <cmath>

#include "vec3.h"
#include "math_utils.h"

struct quaternion
{
    float w, x, y, z;

    quaternion(float w = 0, float x = 0, float y = 0, float z = 0) : w(w), x(x), y(y), z(z) {}

    explicit quaternion(vec3 axis, float rotation)
    {
        axis = axis.normalize();
        float s = sin(radians(rotation)/2);
        float c = cos(radians(rotation)/2);

        w = c; 
        x = axis.x * s;
        y = axis.y * s;
        z = axis.z * s;
    }

    static quaternion normalize(const quaternion& quat)
    {
        float magnitude = sqrt(
            quat.w * quat.w +
            quat.x * quat.x +
            quat.y * quat.y +
            quat.z * quat.z
        );
        if (magnitude == 0) return quaternion(0.0f, 0.0f, 0.0f, 0.0f);
        
        quaternion normQuat = {
            quat.w / magnitude,
            quat.x / magnitude,
            quat.y / magnitude,
            quat.z / magnitude
        };
        return normQuat;
    }

    static quaternion clamp(const quaternion& quat)
    {
        quaternion clampQuat = quaternion(0.0f, quat.x, quat.y, quat.z);
        return clampQuat;
    }

    static quaternion conjugate(const quaternion& quat)
    {
        return quaternion(quat.w, -quat.x, -quat.y, -quat.z);
    }	

    quaternion operator*(const quaternion& other) const
    {
        return quaternion(
            w * other.w - x * other.x - y * other.y - z * other.z,
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w
        );
    }

    static vec3 rotateVec3(const quaternion& q, const vec3& vec)
    {
        quaternion qI = {q.w, -q.x, -q.y, -q.z};
        quaternion v = {0.0f, vec.x, vec.y, vec.z};
        quaternion r = (q * v) * qI;
        return vec3(r.x, r.y, r.z);
    }
};

#endif