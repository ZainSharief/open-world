#ifndef MAT4_H
#define MAT4_H

#include <cmath>
#include "vec4.h"
#include "vec3.h"
#include "math_utils.h"

struct mat4 {
    float m[16];

    mat4() 
    {
        for (int i = 0; i < 16; i++) m[i] = 0.0f;
    }

    explicit mat4(float diagonal) 
    {
        for (int i = 0; i < 16; i++) m[i] = 0.0f;
        m[0] = diagonal;
        m[5] = diagonal;
        m[10] = diagonal;
        m[15] = diagonal;
    }

    static mat4 identity() 
    {
        return mat4(1.0f);
    }

    static mat4 translate(const vec3& translation) 
    {
        mat4 result = identity();
        result.m[3] = translation.x;
        result.m[7] = translation.y;
        result.m[11] = translation.z;
        return result;
    }

    static mat4 rotate(float angle, const vec3& axis) 
    {
        vec3 axisNorm = axis.normalize();
        float r = radians(angle);
        float s = sin(r);
        float c = cos(r);
        
        float x = axisNorm.x, y = axisNorm.y, z = axisNorm.z;

        mat4 result = identity();
        result.m[0] = x * x * (1 - c) + c;
        result.m[4] = y * x * (1 - c) + z * s;
        result.m[8] = x * z * (1 - c) - y * s;

        result.m[1] = x * y * (1 - c) - z * s;
        result.m[5] = y * y * (1 - c) + c;
        result.m[9] = y * z * (1 - c) + x * s;

        result.m[2] = x * z * (1 - c) + y * s;
        result.m[6] = y * z * (1 - c) - x * s;
        result.m[10] = z * z * (1 - c) + c;

        return result;
    }

    static mat4 projection(float fov, float aspect, float znear, float zfar)
    {
        float r = (fov / 2) * (PI / 180.0f);
        float t = tan(r);

        mat4 result;
        result.m[0] = 1 / (aspect * t);
        result.m[5] = 1 / t;
        result.m[10] = -(zfar + znear) / (zfar - znear);
        result.m[11] = -(2.0f * zfar * znear) / (zfar - znear);
        result.m[14] = -1.0f;
        return result;
    }
    
    static mat4 lookAt(vec3 position, vec3 target, vec3 up) 
    {
        vec3 direction = (position - target).normalize();
        vec3 right = up.cross(direction).normalize();
        vec3 upOrtho = direction.cross(right).normalize();
        
        mat4 rotation = identity();
        rotation.m[0] = right.x;
        rotation.m[1] = right.y;
        rotation.m[2] = right.z;

        rotation.m[4] = upOrtho.x;
        rotation.m[5] = upOrtho.y;
        rotation.m[6] = upOrtho.z;

        rotation.m[8] = direction.x;
        rotation.m[9] = direction.y;
        rotation.m[10] = direction.z;

        mat4 translation = identity();
        translation.m[3] = -position.x;
        translation.m[7] = -position.y;
        translation.m[11] = -position.z;

        return rotation * translation;
    }

    mat4 operator+(const mat4& other) const 
    {
        mat4 result;
        for (int i = 0; i < 16; i++) {
            result.m[i] = m[i] + other.m[i];
        }
        return result;
    }

    mat4 operator-(const mat4& other) const 
    {
        mat4 result;
        for (int i = 0; i < 16; i++) {
            result.m[i] = m[i] - other.m[i];
        }
        return result;
    }

    mat4 operator*(const mat4& other) const 
    {
        mat4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    result.m[i*4+j] += m[i*4+k] * other.m[k*4+j];
                }
            }
        }
        return result;
    }    

    vec4 operator*(const vec4& v) const 
    {
        return vec4(
            m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3] * v.w,
            m[4] * v.x + m[5] * v.y + m[6] * v.z + m[7] * v.w,
            m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11] * v.w,
            m[12] * v.x + m[13] * v.y + m[14] * v.z + m[15] * v.w
        );
    }
    
    mat4 operator*(const float other) const 
    {
        mat4 result;
        for (int i = 0; i < 16; i++) {
            result.m[i] = m[i] * other;
        }
        return result;
    }

    mat4 operator/(const float other) const 
    {
        
        if (other == 0) {
            std::cerr << "WARNING: DIVISION mat4 BY 0 WITH A float" << std::endl;
            return mat4();
        } 

        mat4 result;
        for (int i = 0; i < 16; i++) {
            result.m[i] = m[i] / other;
        }
        return result;
    }

    void print()
    {
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                std::cout << m[col * 4 + row] << " ";
            }
            std::cout << "" << std::endl;
        }
    }
};

#endif