/*
** Ships
*/

#include "../../include/Systems/Ships.hpp"

Ships::Ships()
{
}

Ships::~Ships()
{
}


void Ships::Update(ECS::ECS &ecs, ECS::SystemID thisID, uint32_t msecs)
{
    std::vector<ECS::EntityID> entities = ecs.getEntitiesByComponentsAllOf<ShipComponent, Position, Quaternion>();
    const MouseState &mouse = ecs.entityGetComponent<MouseState>(ecs.getEntitiesByComponentsAllOf<MouseState>()[0]);
    float deltaTime = msecs / 1000.0f;
    bool dragging =
        (mouse.left_button.is_pressed || mouse.left_button.released_this_frame)
        && length(mouse.left_button.position_when_pressed - mouse.position) >= 2.5f;

    for (auto &it : entities) {
        ShipComponent &ship = ecs.entityGetComponent<ShipComponent>(it);
        Position &pos = ecs.entityGetComponent<Position>(it);
        // Quaternion &rot = ecs.entityGetComponent<Quaternion>(it);

        if (!dragging && mouse.left_button.released_this_frame) {
            if (CheckCollisionPointRec(mouse.position, ship.hitbox)) {
                ship.is_selected = true;
            } else {
                ship.is_selected = false;
            }
        }

        if (ship.is_selected && !dragging && mouse.left_button.released_this_frame) {
            Vector2 mousePos = GetMousePosition();
            ship.move_to = {mousePos.x, mousePos.y};
            ship.is_moving = true;
        }

        // Temporary autopilot calculations:
        if (ship.is_moving) {
            Vector2 direction = ship.move_to - pos;
            float distance = length(direction);
            if (distance > 1.0f) {
                direction = normalize(direction);
                ship.velocity = direction * ship.max_speed;
            } else {
                ship.is_moving = false;
                ship.velocity = {0.0f, 0.0f};
            }

            pos.x += ship.velocity.x * deltaTime;
            pos.y += ship.velocity.y * deltaTime;

            bool overshot = length((ship.move_to - pos)) > distance;
            if (overshot) {
                pos.x = ship.move_to.x;
                pos.y = ship.move_to.y;
                ship.is_moving = false;
                ship.velocity = {0.0f, 0.0f};
            }
        }

        // To implement:
        // Autopilot

        // Calculates which thrusters to activate based on the ship's current position and the target position
        // if (ship.is_moving) {}

        // Adds forces to the thrusters that are active (with AccelForce)
        // Reduces forces to the thrusters that are inactive (with DecelForce)
        // Applies thrusters forces to the velocity
        // Applies damping to the velocity
        // Moves the ship based on the velocity and deltaTime

        
    }
}

ECS::EntityID Ships::createShip(ECS::ECS &ecs)
{
    ECS::EntityID ship = ecs.entityCreate();
    Texture2D text = LoadTexture(GET_ASSET_PATH("ship_placeholder.png").c_str());

    ecs.entityAddComponent<ShipComponent>(ship).hitbox = {0.0f, 0.0f, (float)text.width, (float)text.height};
    ecs.entityAddComponent<Position>(ship) = {250.0f, 250.0f};
    ecs.entityAddComponent<Quaternion>(ship);
    ecs.entityAddComponent<Texture2D>(ship) = text;
    // ecs.entitySetGroup(ship, ECS::EntityGroup::SHIPS);
    return ship;
}
