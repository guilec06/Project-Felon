/*
** Utils
*/

#ifndef UTILS_HPP_
    #define UTILS_HPP_
#include "ProjectComponents.hpp"
#include <cmath>

Vector2 operator+(const Vector2 &a, const Vector2 &b);
Vector2 operator-(const Vector2 &a, const Vector2 &b);
Vector2 operator*(const Vector2 &a, const Vector2 &b);
Vector2 operator/(const Vector2 &a, const Vector2 &b);
Vector2 operator*(const Vector2 &a, float b);
bool operator==(const Vector2 &a, const Vector2 &b);
float length(const Vector2 &a);
Vector2 normalize(const Vector2 &a);

#endif /* !UTILS_HPP_ */
