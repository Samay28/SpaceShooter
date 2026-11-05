#include "CollisionSystem.h"
#include "Spatial/SpatialGrid.h"    
#include "raymath.h"
#include "raylib.h"

CollisionSystem::CollisionSystem()
{   
    playerHitSound = LoadSound("assets/sounds/player_hit.wav");
    shieldBreakSound = LoadSound("assets/sounds/shield_break.wav");
}

CollisionSystem::~CollisionSystem()
{
    // Unload the sound effect when the collision system is destroyed
    //it is important to unload the sound effect to free up memory and resources used by the sound system
    UnloadSound(playerHitSound);
    UnloadSound(shieldBreakSound);
}

void CollisionSystem::Update(GameWorld& world, Player& player, const EnemyDatabase& enemyDatabase, const SpatialGrid& spatialGrid)
{
    // Check for collisions between projectiles and enemies

    for (size_t projectileIndex = 0; projectileIndex < world.projectiles.size(); ++projectileIndex)
    {
        const Projectile& projectile = world.projectiles[projectileIndex];
        const Vector2 projectilePostion = world.projectilePositions[projectileIndex].value;

        //-------------------- Player Projectile vs Enemy Collision --------------------
        if (projectile.owner == ProjectileOwner::Player || projectile.owner == ProjectileOwner::Ally)
        {
            //ask the grid for only enemies near the projectile, to reduce the number of checks needed
            const std::vector<size_t> candidates = spatialGrid.QueryEnemiesInRadius(projectilePostion, 4.f);

            //now perform collision checks only on the candidates returned by the spatial grid
            //instead of for (size_t i = 0; i < world.enemies.size(); ++i)
            for (const size_t i : candidates)
            {
                const Enemy& enemy = world.enemies[i];
                const Vector2 enemyPosition = world.enemyPositions[i].value;

                const float distance = Vector2Distance(projectilePostion, enemyPosition);
                const EnemyDefinition& definition = enemyDatabase.Get(enemy.type);

                if (distance <= definition.radius + 4.f && !enemy.isImposter)
                {
                    // Collision detected, apply damage to the enemy
                    world.enemyHealth[i].currentHealth -= projectile.damage;
                    // Mark the projectile for removal by setting its lifetime to 0
                    world.projectiles[projectileIndex].lifetime = 0.0f;
                    PlaySound(playerHitSound);

                    break; // Exit the loop since the projectile can only hit one enemy
                }
            }
        }

        //-------------------- Enemy Projectile vs Player Collision --------------------
        else if (projectile.owner == ProjectileOwner::Enemy)
        {
            const Vector2 playerPosition = player.GetPosition();

            // Keep track of the closest target that this projectile overlaps.
            float closestDistanceSq = FLT_MAX;

            bool hitPlayer = false;
            size_t hitAllyIndex = 0;

            // -----------------------------------------
            // 1. Check collision against the player.
            // -----------------------------------------

            const float playerDistanceSq =
                Vector2DistanceSqr(projectilePostion, playerPosition);

            // Preserve your existing player collision radius.
            if (playerDistanceSq <= 19.0f * 19.0f)
            {
                closestDistanceSq = playerDistanceSq;
                hitPlayer = true;
            }

            // -----------------------------------------
            // 2. Check collision against all allies.
            // -----------------------------------------

            for (size_t i = 0; i < world.allies.size(); ++i)
            {
                // Dead allies should not receive further damage.
                if (world.allyHealth[i].currentHealth <= 0.0f)
                    continue;

                const Ally& ally = world.allies[i];

                const Vector2 allyPosition =
                    world.allyPositions[i].value;

                // Retrieve the collision radius associated with this ally type.
                const EnemyDefinition& definition =
                    enemyDatabase.Get(ally.type);

                const float distanceSq =
                    Vector2DistanceSqr(projectilePostion, allyPosition);

                // Combined ally and projectile collision radius.
                const float collisionRadius = definition.radius + 4.0f;

                if (distanceSq <= collisionRadius * collisionRadius &&
                    distanceSq < closestDistanceSq)
                {
                    closestDistanceSq = distanceSq;

                    hitPlayer = false;
                    hitAllyIndex = i;
                }
            }

            // -----------------------------------------
            // 3. Apply damage to the closest target.
            // -----------------------------------------

            if (closestDistanceSq != FLT_MAX)
            {
                if (hitPlayer)
                {
                    // Player shield only protects the player.
                    if (HasShield(world))
                    {
                        RemoveShield(world);
                        PlaySound(shieldBreakSound);
                    }
                    else
                    {
                        player.TakeDamage(projectile.damage);
                        PlaySound(playerHitSound);
                    }
                }
                else
                {
                    // Apply damage to the selected ally.
                    world.allyHealth[hitAllyIndex].currentHealth -=
                        projectile.damage;

                    PlaySound(playerHitSound);
                }

                // This projectile can only hit one target.
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
            //play audio
            return;
        }
    }
}
