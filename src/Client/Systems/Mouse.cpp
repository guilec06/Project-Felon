/*
** EPITECH PROJECT, 2025
** project01
** File description:
** Mouse
*/

#include "Mouse.hpp"

Mouse::Mouse()
{
}

Mouse::~Mouse()
{
}

void Mouse::Update(ECS::ECS &ecs, ECS::SystemID thisID, uint32_t msecs)
{
    std::vector<ECS::EntityID> entities = ecs.getEntitiesByComponentsAllOf<MouseState>();
    MouseState &mouse = ecs.entityGetComponent<MouseState>(entities[0]);
    Vector2 mousePos = GetMousePosition();
    float deltaTime = msecs / 1000.0f;

    mouse.delta = {mousePos.x - mouse.position.x, mousePos.y - mouse.position.y};
    mouse.position = mousePos;
    mouse.is_moving = (mouse.delta.x != 0 || mouse.delta.y != 0);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        mouse.left_button.is_pressed = true;
        mouse.left_button.position_when_pressed = mouse.position;
        mouse.left_button.pressed_this_frame = true;
    } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        mouse.left_button.is_pressed = false;
        mouse.left_button.position_when_released = mouse.position;
        mouse.left_button.released_this_frame = true;
    } else {
        mouse.left_button.pressed_this_frame = false;
        mouse.left_button.released_this_frame = false;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        mouse.right_button.is_pressed = true;
        mouse.right_button.position_when_pressed = mouse.position;
        mouse.right_button.pressed_this_frame = true;
    } else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
        mouse.right_button.is_pressed = false;
        mouse.right_button.position_when_released = mouse.position;
        mouse.right_button.released_this_frame = true;
    } else {
        mouse.right_button.pressed_this_frame = false;
        mouse.right_button.released_this_frame = false;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
        mouse.middle_button.is_pressed = true;
        mouse.middle_button.position_when_pressed = mouse.position;
        mouse.middle_button.pressed_this_frame = true;
    } else if (IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE)) {
        mouse.middle_button.is_pressed = false;
        mouse.middle_button.position_when_released = mouse.position;
        mouse.middle_button.released_this_frame = true;
    } else {
        mouse.middle_button.pressed_this_frame = false;
        mouse.middle_button.released_this_frame = false;
    }
}
