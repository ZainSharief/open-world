#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

struct vec3 {
    float x, y, z;

    vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    
    // override the + operator
    vec3 operator+(const vec3& other) const {
        return vec3(x + other.x, y + other.y, z + other.z);
    }
    vec3& operator+=(const vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    // override the - operator
    vec3 operator-(const vec3& other) const {
        return vec3(x - other.x, y - other.y, z - other.z);
    }
    vec3& operator-=(const vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    vec3 operator-() const {
        return vec3(-x, -y, -z);
    }

    // Override the * operator
    vec3 operator*(const vec3& other) const {
        return vec3(x * other.x, y * other.y, z * other.z);
    }
    // Override the * operator
    vec3 operator*(const float& other) const {
        return vec3(x * other, y * other, z * other);
    }

    // Override the / operator
    vec3 operator/(const vec3& other) const {
        // avoids division by 0 
        if (other.x == 0 || other.y == 0 || other.z == 0) {
            std::cerr << "WARNING: DIVISION vec3 BY 0 WITH A vec3" << std::endl;
            return vec3(
                other.x == 0 ? x / 1e-8 : x / other.x, 
                other.y == 0 ? y / 1e-8 : y / other.y,
                other.z == 0 ? z / 1e-8 : z / other.z
            );
        }
        return vec3(x / other.x, y / other.y, z / other.z);
    }
    vec3 operator/(const float& other) const {
        // avoids division by 0 
        if (other == 0) {
            std::cerr << "WARNING: DIVISION vec3 BY 0 WITH A FLOAT" << std::endl;
            return vec3(1e8, 1e8, 1e8);
        }
        return vec3(x / other, y / other, z / other);
    }

    // Normalise to a unit vector
    vec3 normalize() const {
        float magnitude = std::sqrt(x*x + y*y + z*z);
        return *this / magnitude;
    }

    // Compute a dot product between vectors
    float dot(const vec3& other) {
        return x * other.x + y * other.y + z * other.z;
    }

    vec3 cross(const vec3& other) {
        return vec3(
            y * other.z - z * other.y, 
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
};

#endif