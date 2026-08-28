#include "AISystem.h"
#include "raymath.h"

void AISystem::Update(GameWorld& world, Vector2 playerPosition, float deltaTime)
{
    for (size_t i = 0; i < world.enemies.size(); ++i)
    {
        UpdateEnemy(
            world,
            i,
            playerPosition,
            deltaTime);
    }
}

void AISystem::UpdateEnemy(GameWorld& world, size_t index, Vector2 playerPosition, float deltaTime)
{
    Enemy& enemy = world.enemies[index];
    Position& position =
        world.enemyPositions[index];
    Velocity& velocity =
        world.enemyVelocities[index];

    enemy.stateTimer -= deltaTime;


    if (enemy.state == EnemyState::Moving)
    {
        Vector2 target = { playerPosition.x, 200.f }; // Move towards a point above the player
        Vector2 direction = Vector2Subtract(target, position.value);

        if (Vector2Length(direction) > 1.f)
        {
            direction = Vector2Normalize(direction); // Normalize the direction vector 
            velocity.value = Vector2Scale(direction, enemy.moveSpeed); // Set the velocity based on the direction and speed
        }  

        if (position.value.y >= 150.f)
        {   
            // If the enemy has reached a certain height, switch to attacking state
            enemy.state = EnemyState::Attacking;

            enemy.stateTimer = enemy.attackDuration; // Reset the state timer for attacking

            velocity.value = { 0.f, 0.f }; // Stop moving when attacking
        }
    }
    else if (enemy.state == EnemyState::Attacking)
    {
        velocity.value = { 0.f, 0.f }; // Stop moving when attacking

        if(enemy.stateTimer <= 0.f)
        {
            // After attacking, switch back to moving state
            enemy.state = EnemyState::Moving;
            enemy.stateTimer = 0.f; // Reset the state timer for moving
        }
    }
}
