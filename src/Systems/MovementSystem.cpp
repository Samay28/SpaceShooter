#include "MovementSystem.h"


void MovementSystem::Update(GameWorld& world, float deltaTime)
{   
    //---------------- Projectiles ----------------
    for (size_t i = 0; i < world.projectilePositions.size(); i++)
    {
        // Update position based on velocity and deltaTime
        world.projectilePositions[i].value.x += world.projectileVelocities[i].value.x * deltaTime;
        world.projectilePositions[i].value.y += world.projectileVelocities[i].value.y * deltaTime;
    }

    //---------------- Enemies ----------------
    for (size_t i = 0; i < world.enemies.size(); i++)
    {
        world.enemyPositions[i].value.x += world.enemyVelocities[i].value.x * deltaTime;
        world.enemyPositions[i].value.y += world.enemyVelocities[i].value.y * deltaTime;
    }


}
