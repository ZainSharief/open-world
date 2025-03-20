#ifndef MATH_UTILS_H
#define MATH_UTILS_H

constexpr float PI = 3.14159265359f;

float radians(float degrees)
{
    return degrees * (PI / 180.0f);
}

#endif