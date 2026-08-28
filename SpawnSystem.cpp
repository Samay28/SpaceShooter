#include "SpawnSystem.h"
#include "raylib.h"

void SpawnSystem::Update(GameWorld& world, float deltaTime)
{
    m_spawnTimer -= deltaTime;

    if(m_spawnTimer <= 0.0f)
    {
        SpawnEnemy(world);
        m_spawnTimer = m_spawnInterval;
    }
}

void SpawnSystem::SpawnEnemy(GameWorld& world)
{   
    // Create a new enemy and set its properties
    Enemy enemy{};

    enemy.type = EnemyType::Basic;
    enemy.state = EnemyState::Moving;

    enemy.stateTimer = 0.0f;
    enemy.attackDuration = 2.0f;

    enemy.moveSpeed = 80.0f;
    enemy.health = 50.0f;
    enemy.radius = 15.0f;

    // Create a new health component for the enemy
    Health health{};
    health.currentHealth = enemy.health;
    health.MaxHealth = enemy.health;

    // Create a new position and velocity for the enemy
    Position position{};
    position.value = {
        static_cast<float>(GetRandomValue(50, GetScreenWidth() - 50)),
        -30.0f
    };

    Velocity velocity{};
    velocity.value = { 0.0f, enemy.moveSpeed };


    // Add the enemy, health, position, and velocity to the world
    world.enemies.push_back(enemy);
    world.enemyHealth.push_back(health);

    world.enemyPositions.push_back(position);
    world.enemyVelocities.push_back(velocity);
}
