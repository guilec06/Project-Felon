/*
** Ships
*/

#ifndef SHIPS_HPP_
    #define SHIPS_HPP_

#include "../Project01.hpp"
#include "Mouse.hpp"

struct ShipComponent {
    Hitbox hitbox;
    Position move_to = {0.0, 0.0};
    Vector2 velocity = {0.0, 0.0};
    bool is_selected = false;
    bool is_moving = false;
    float weight = 1.0f;        // tons, Weight of the ship, affects acceleration and decelration
    float damping = 0.1f;       // Damping factor to reduce velocity over time
    bool rotation_locked = false;   // Is the ship's rotation locked? If true, the ship will not rotate
    float max_speed = 100.0f;      // Maximum speed of the ship in m/s (for testing purposes, this variable is temporary)
    struct {
        float strafeRightForce = 0.0f;           // Newtons, force applied when strafing to the right
        float strafeLeftForce = 0.0f;            // Newtons, force applied when strafing to the left
        float forwardForce = 0.0f;          // Newtons, force applied when moving forward 
        float backwardForce = 0.0f;         // Newtons, force applied when moving backward
        float rotateRightForce = 0.0f;           // Newtons, force applied when rotating the ship to the right
        float rotateLeftForce = 0.0f;            // Newtons, force applied when rotating the ship to the left
        float strafeMaxForce = 1000.0f;        // Newtons, maximum force applied when strafing
        float forwardMaxForce = 1000.0f;       // Newtons, maximum force applied when moving forward
        float backwardMaxForce = 1000.0f;      // Newtons, maximum force applied when moving backward
        float rotateMaxForce = 1000.0f;        // Newtons, maximum force applied when rotating the ship
        float strafeAccelForce = 100.0f;      // Newtons/s, acceleration when strafing
        float forwardAccelForce = 100.0f;     // Newtons/s, acceleration when moving forward
        float backwardAccelForce = 100.0f;    // Newtons/s, acceleration when moving backward
        float rotateAccelForce = 100.0f;      // Newtons/s, acceleration when rotating the ship
        float strafeDecelForce = 250.0f;       // Newtons/s, deceleration when strafing
        float forwardDecelForce = 250.0f;      // Newtons/s, deceleration when moving forward
        float backwardDecelForce = 250.0f;     // Newtons/s, deceleration when moving backward
        float rotateDecelForce = 250.0f;       // Newtons/s, deceleration when rotating the ship
        bool thrusters_forward_active = false;  // Is the forward thruster active?
        bool thrusters_backward_active = false; // Is the backward thruster active?
        bool thrusters_strafe_right_active = false; // Is the right strafe thruster active?
        bool thrusters_strafe_left_active = false;  // Is the left strafe thruster active?
        bool thrusters_rotate_right_active = false; // Is the right rotation thruster active?
        bool thrusters_rotate_left_active = false;  // Is the left rotation thruster active?
    } thrusters;
};

class Ships : public ECS::ISystem {
    public:
        Ships();
        ~Ships();

        void Update(ECS::ECS &ecs, ECS::SystemID thisID, uint32_t msecs) override;
        static ECS::EntityID createShip(ECS::ECS &ecs);
    protected:
    private:
};

#endif /* !SHIPS_HPP_ */
