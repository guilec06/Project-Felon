/*
** EPITECH PROJECT, 2025
** project01
** File description:
** Utils
*/

#include "Utils.hpp"

Vector2 operator+(const Vector2 &a, const Vector2 &b) {
    return {a.x + b.x, a.y + b.y};
}
Vector2 operator-(const Vector2 &a, const Vector2 &b) {
    return {a.x - b.x, a.y - b.y};
}
Vector2 operator*(const Vector2 &a, const Vector2 &b) {
    return {a.x * b.x, a.y * b.y};
}
Vector2 operator/(const Vector2 &a, const Vector2 &b) {
    return {a.x / b.x, a.y / b.y};
}
Vector2 operator*(const Vector2 &a, float b) {
    return {a.x * b, a.y * b};
}

bool operator==(const Vector2 &a, const Vector2 &b) {
    return (a.x == b.x && a.y == b.y);
}

float length(const Vector2 &a) {
    return sqrtf(a.x * a.x + a.y * a.y);
}
Vector2 normalize(const Vector2 &a) {
    float len = length(a);
    if (len == 0.0f) return {0.0f, 0.0f};
    return {a.x / len, a.y / len};
}
