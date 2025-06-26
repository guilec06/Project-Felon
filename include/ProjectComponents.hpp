/*
** EPITECH PROJECT, 2025
** project01
** File description:
** ProjectComponents
*/

#ifndef PROJECTCOMPONENTS_HPP_
    #define PROJECTCOMPONENTS_HPP_

#include <raylib.h>

// Same as Vector3
typedef Vector2 Position;
// Same as Rectangle
typedef Rectangle Hitbox;
// 2 Vector3 storing acceleration and velocity
struct Physics {
    Vector2 acceleration;
    Vector2 velocity;
};
struct CameraMovement {
    bool IsDragging = false;
    Vector2 DragStart = {0.0, 0.0};
    Vector2 TargetStart = {0.0, 0.0};
};

#endif /* !PROJECTCOMPONENTS_HPP_ */
