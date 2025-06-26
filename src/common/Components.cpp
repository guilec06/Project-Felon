/*
** project01
*/
#include "../include/Project01.hpp"

void LoadComponents(ECS::ECS &ecs)
{
    ecs.registerComponent<Camera2D>();
    ecs.registerComponent<Transform>();
    ecs.registerComponent<Position>();
    ecs.registerComponent<Hitbox>();
    ecs.registerComponent<Physics>();
    ecs.registerComponent<CameraMovement>();
    ecs.registerComponent<Quaternion>();
    ecs.registerComponent<Texture2D>();
    ecs.registerComponent<ShipComponent>();
    ecs.registerComponent<MouseState>();
}
