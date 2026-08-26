#include "MovementSystem.h"

void MovementSystem::Update(GameWorld& world, float deltaTime)
{
    for (size_t i = 0; i < world.positions.size(); i++)
    {
        // Update position based on velocity and deltaTime
        world.positions[i].value.x += world.velocities[i].value.x * deltaTime;
        world.positions[i].value.y += world.velocities[i].value.y * deltaTime;
    }
}
