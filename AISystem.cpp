#include "AISystem.h"
#include "raymath.h"

void AISystem::Update(GameWorld& world, const EnemyDatabase& enemyDatabase, Vector2 playerPosition, float deltaTime)
{
    for (size_t i = 0; i < world.enemies.size(); ++i)
    {
        Enemy& enemy = world.enemies[i];
        Position& position = world.enemyPositions[i];
        Velocity& velocity = world.enemyVelocities[i];

        const EnemyDefinition& definition = enemyDatabase.Get(enemy.type);
        
        enemy.stateTimer -= deltaTime;

        //---------------- Moving ----------------

        if (enemy.state == EnemyState::Moving)
        {
            Vector2 dir = Vector2Subtract(enemy.targetPosition, position.value);

            const float distance = Vector2Length(dir);

            
            if (distance > 5.f)
            {
                dir = Vector2Normalize(dir);

                velocity.value = Vector2Scale(dir, definition.moveSpeed);
            }
            else
            {
                velocity.value = { 0.f, 0.f };
                enemy.state = EnemyState::Attacking;
                enemy.stateTimer = RandomAttackDuration(definition);

                enemy.weaponCooldown = 0.f; // Reset weapon cooldown when switching to attacking state
            }
        }
        //----------------Attacking----------------
        else if (enemy.state == EnemyState::Attacking)
        {
            velocity.value =
            { 0.0f, 0.0f };

            if (enemy.stateTimer <= 0.0f)
            {
                ChooseNewTarget(world, i);

                enemy.state = EnemyState::Moving;
            }
        }
    }
}

void AISystem::ChooseNewTarget(GameWorld& world, size_t index)
{
    constexpr float minDistance = 50.0f; //constexpr used for compile-time constants, which can improve performance and readability.
    Vector2 newTarget{};

    bool foundTarget = false;

    //----------------- Try to find a new target position that is at least minDistance away from the current position -----------------
    for (int attempt = 0; attempt < 10; ++attempt)
    {   
        //----------------- Generate a random target position -----------------
        newTarget = { static_cast<float>(GetRandomValue(50, GetScreenWidth() - 50)),
                      static_cast<float>(GetRandomValue(100, 350)) };

        bool valid = true;
        for (size_t i = 0; i < world.enemyPositions.size(); ++i)
        {
            if (i == index)
            {
                continue; // Skip the current enemy
            }

            const float distance = Vector2Distance(newTarget, world.enemyPositions[i].value);
            if(distance < minDistance)
            {
                valid = false;
                break; // No need to check further if we found a close enemy
            }
        }

        if(valid)
        {
            foundTarget = true;
            break; // Found a valid target position
        }
    }
    if (!foundTarget)
    {   
        // If no valid target was found after 10 attempts, just assign a random position without checking distance
        newTarget = {
            static_cast<float>(
                GetRandomValue(50, GetScreenWidth() - 50)
            ),
            static_cast<float>(
                GetRandomValue(100, 350)
            )
        };
    }

    world.enemies[index].targetPosition = newTarget;
}

float AISystem::RandomAttackDuration(const EnemyDefinition& definition)
{
    return GetRandomValue( static_cast<int>(definition.attackDurationMin * 100.f),
                           static_cast<int>(definition.attackDurationMax * 100.f)) / 100.f;
}



