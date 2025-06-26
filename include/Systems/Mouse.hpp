/*
** EPITECH PROJECT, 2025
** project01
** File description:
** Mouse
*/

#ifndef MOUSE_HPP_
    #define MOUSE_HPP_

#include "../Project01.hpp"

struct MouseButtonState {
    Vector2 position_when_pressed;
    Vector2 position_when_released;
    bool is_pressed;
    bool pressed_this_frame;
    bool released_this_frame;
};

struct MouseState {
    Vector2 position;
    Vector2 delta; // Change in position since last frame
    bool is_moving; // Is the mouse currently moving?
    MouseButtonState left_button;
    MouseButtonState right_button;
    MouseButtonState middle_button;
};

class Mouse : public ECS::ISystem {
    public:
        Mouse();
        ~Mouse();

        void Update(ECS::ECS &ecs, ECS::SystemID thisID, uint32_t msecs) override;
    protected:
    private:
};
#endif /* !MOUSE_HPP_ */
