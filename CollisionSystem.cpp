#include "CollisionSystem.h"
#include "raymath.h"

void CollisionSystem::Update(GameWorld& world)
{
    // Check for collisions between projectiles and enemies

    for (size_t projectileIndex = 0; projectileIndex < world.projectiles.size(); ++projectileIndex)
    {
        const Vector2 projectilePostion = world.projectilePositions[projectileIndex].value;

        for (size_t enemyIndex = 0; enemyIndex < world.enemies.size(); ++enemyIndex)
        {   
            // Get the enemy's position
            const Vector2 enemyPosition = world.enemyPositions[enemyIndex].value;

            // Calculate the distance between the projectile and the enemy
            float distance = Vector2Distance(projectilePostion, enemyPosition);

            // Check if the distance is less than the sum of their radii (collision detection)
            if (distance <= world.enemies[enemyIndex].radius + 4.f)
            {
                world.enemyHealth[enemyIndex].currentHealth -= world.projectiles[projectileIndex].damage;

                world.projectiles[projectileIndex].lifetime = 0.0f; // Mark the projectile for removal
                break; // Exit the inner loop since the projectile has hit an enemy
            }
        }
    }
}
