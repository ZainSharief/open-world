#ifndef VEC4_H
#define VEC4_H

#include <cmath>
#include <iostream>

struct vec4 {
    float x, y, z, w;

    vec4(float x = 0, float y = 0, float z = 0, float w = 0) : x(x), y(y), z(z), w(w) {}
    
    // override the + operator
    vec4 operator+(const vec4& other) const 
    {
        return vec4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    // override the - operator
    vec4 operator-(const vec4& other) const 
    {
        return vec4(x - other.x, y - other.y, z - other.z, w - other.w);
    }
    vec4 operator-() const 
    {
        return vec4(-x, -y, -z, -w);
    }

    // Override the * operator
    vec4 operator*(const vec4& other) const 
    {
        return vec4(x * other.x, y * other.y, z * other.z, w * other.w);
    }
    // Override the * operator
    vec4 operator*(const float& other) const 
    {
        return vec4(x * other, y * other, z * other, w * other);
    }

    // Override the / operator
    vec4 operator/(const vec4& other) const 
    {
        // avoids division by 0 
        if (other.x == 0 || other.y == 0 || other.z == 0 || other.w == 0) {
            std::cerr << "WARNING: DIVISION vec4 BY 0 WITH A VEC4" << std::endl;
            return vec4(
                other.x == 0 ? x / 1e-8 : x / other.x, 
                other.y == 0 ? y / 1e-8 : y / other.y,
                other.z == 0 ? z / 1e-8 : z / other.z,
                other.w == 0 ? w / 1e-8 : w / other.w
            );
        }
        return vec4(x / other.x, y / other.y, z / other.z, w / other.w);
    }
    vec4 operator/(const float& other) const 
    {
        // avoids division by 0 
        if (other == 0) {
            std::cerr << "WARNING: DIVISION vec4 BY 0 WITH A FLOAT" << std::endl;
            return vec4(1e8, 1e8, 1e8, 1e8);
        }
        return vec4(x / other, y / other, z / other, w / other);
    }

    // Normalise to a unit vector
    vec4 normalize() const 
    {
        float magnitude = std::sqrt(x*x + y*y + z*z + w*w);
        return *this / magnitude;
    }

    // Compute a dot product between vectors
    float dot(const vec4& other) const 
    {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }
};

#endif