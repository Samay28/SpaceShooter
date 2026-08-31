#include "CollisionSystem.h"
#include "raymath.h"

void CollisionSystem::Update(GameWorld& world, Player& player, const EnemyDatabase& enemyDatabase)
{
    // Check for collisions between projectiles and enemies

    for (size_t projectileIndex = 0; projectileIndex < world.projectiles.size(); ++projectileIndex)
    {   
        const Projectile& projectile = world.projectiles[projectileIndex];
        const Vector2 projectilePostion = world.projectilePositions[projectileIndex].value;

        //-------------------- Player Projectile vs Enemy Collision --------------------
        if (projectile.owner == ProjectileOwner::Player)
        {
            for (size_t i = 0; i < world.enemies.size(); ++i)
            {
                const Enemy& enemy = world.enemies[i];
                const Vector2 enemyPosition = world.enemyPositions[i].value;

                const float distance = Vector2Distance(projectilePostion, enemyPosition);
                const EnemyDefinition& definition = enemyDatabase.Get(enemy.type);

                if (distance <= definition.radius + 4.f) 
                {
                    // Collision detected, apply damage to the enemy
                    world.enemyHealth[i].currentHealth -= projectile.damage;
                    // Mark the projectile for removal by setting its lifetime to 0
                    world.projectiles[projectileIndex].lifetime = 0.0f;

                    break; // Exit the loop since the projectile can only hit one enemy
                }
            }
        }

        //-------------------- Enemy Projectile vs Player Collision --------------------
        else
        {
            const Vector2 playerPosition = player.GetPosition();
            const float distance = Vector2Distance(projectilePostion, playerPosition);

            if(distance <= 15.f)
            {
                // Collision detected, apply damage to the player
                //but only if the player doesn't have a shield
                if(HasShield(world))
                {
                    RemoveShield(world);
                }
                else
                {
                    player.TakeDamage(projectile.damage);
                }
                // Mark the projectile for removal by setting its lifetime to 0
                world.projectiles[projectileIndex].lifetime = 0.0f;
            }
        }
    }
}

bool CollisionSystem::HasShield(const GameWorld& world) const
{
    for (const ActivePowerup& powerup : world.playerPowerups)
    {
        if(powerup.type == PowerupType::Shield)
        {
            return true;
        }
    }
    return false;
}

void CollisionSystem::RemoveShield(GameWorld& world)
{
    for(size_t i= 0; i < world.playerPowerups.size(); ++i)
    {
        if(world.playerPowerups[i].type == PowerupType::Shield)
        {
            world.playerPowerups.erase(world.playerPowerups.begin() + i);
            return;
        }
    }
}
