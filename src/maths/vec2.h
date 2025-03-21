#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <iostream>

struct vec2 {
    float x, y;

    vec2(float x = 0, float y = 0) : x(x), y(y) {}
    
    // override the + operator
    vec2 operator+(const vec2& other) const 
    {
        return vec2(x + other.x, y + other.y);
    }
    vec2& operator+=(const vec2& other) 
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    // override the - operator
    vec2 operator-(const vec2& other) const 
    {
        return vec2(x - other.x, y - other.y);
    }
    vec2& operator-=(const vec2& other) 
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    vec2 operator-() const 
    {
        return vec2(-x, -y);
    }

    // Override the * operator
    vec2 operator*(const vec2& other) const 
    {
        return vec2(x * other.x, y * other.y);
    }
    // Override the * operator
    vec2 operator*(const float& other) const 
    {
        return vec2(x * other, y * other);
    }

    // Override the / operator
    vec2 operator/(const vec2& other) const 
    {
        // avoids division by 0 
        if (other.x == 0 || other.y == 0) {
            std::cerr << "WARNING: DIVISION vec3 BY 0 WITH A vec3" << std::endl;
            return vec2(
                other.x == 0 ? x / 1e-8 : x / other.x, 
                other.y == 0 ? y / 1e-8 : y / other.y
            );
        }
        return vec2(x / other.x, y / other.y);
    }
    vec2 operator/(const float& other) const 
    {
        // avoids division by 0 
        if (other == 0) {
            std::cerr << "WARNING: DIVISION vec3 BY 0 WITH A FLOAT" << std::endl;
            return vec2(1e8, 1e8);
        }
        return vec2(x / other, y / other);
    }

    // Normalise to a unit vector
    vec2 normalize() const 
    {
        float magnitude = std::sqrt(x*x + y*y);
        return *this / magnitude;
    }

    // Compute a dot product between vectors
    float dot(const vec2& other) 
    {
        return x * other.x + y * other.y;
    }
};

#endif