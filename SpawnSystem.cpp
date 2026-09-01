#include "SpawnSystem.h"
#include "Powerup.h"
#include "raymath.h"

void SpawnSystem::Update(GameWorld& world, const EnemyDatabase& enemyDatabase, float deltaTime)
{
    m_spawnTimer -= deltaTime;

    //-------------- Enemy spawning logic ----------------
    if(m_spawnTimer <= 0.0f)
    {
        SpawnEnemy(world, enemyDatabase);
        m_spawnTimer = m_spawnInterval;
    }

    //-------------- Powerup spawning logic ----------------
    m_powerupTimer -= deltaTime;
    if(m_powerupTimer <= 0.0f)
    {
        SpawnPowerup(world);
        m_powerupTimer = m_powerupInterval;
    }
}

void SpawnSystem::SpawnEnemy(GameWorld& world, const EnemyDatabase& enemyDatabase)
{   
    if(world.enemies.size() >= m_maxEnemies)
    {
        return; // Don't spawn more enemies if we reached the max limit
    }
    // Randomly select an enemy type
    const int typeIndex = GetRandomValue(0, static_cast<int>(EnemyType::Tank)); //Because Tank is the last enum value, we can use it to get the range of enemy types
    const EnemyType type = static_cast<EnemyType>(typeIndex);

    // Get the enemy definition from the database
    const EnemyDefinition& definition = enemyDatabase.Get(type); 

    //create a new enemy instance
    Enemy enemy{};
    enemy.type = type;
    enemy.state = EnemyState::Moving;

    enemy.stateTimer = 0.0f;
    enemy.weaponCooldown = 0.0f;

    Position position{};
    // Set the enemy's initial position to a random x value at the top of the screen
    position.value = {
       static_cast<float>(
           GetRandomValue(40, GetScreenWidth() - 40)
       ),
       -30.0f
    };
    //set enemy's target position to a random x value and a random y value 
    enemy.targetPosition = {
      static_cast<float>(
          GetRandomValue(50, GetScreenWidth() - 50)
      ),
      static_cast<float>(
          GetRandomValue(100, 300)
      )
    };


    Vector2 direction =
        Vector2Subtract(
            enemy.targetPosition,
            position.value
        );

    if (Vector2Length(direction) > 0.0f)
    {
        direction =
            Vector2Normalize(direction);
    }


    Velocity velocity{};

    velocity.value =
        Vector2Scale(
            direction,
            definition.moveSpeed
        );

    Health health{};

    health.currentHealth =
        definition.maxHealth;

    health.MaxHealth =
        definition.maxHealth;

    // Add the enemy, health, position, and velocity to the world
    world.enemies.push_back(enemy);
    world.enemyHealth.push_back(health);

    world.enemyPositions.push_back(position);
    world.enemyVelocities.push_back(velocity);
}

void SpawnSystem::SpawnPowerup(GameWorld& world)
{
    const int typeIndex = GetRandomValue(0, static_cast<int>(PowerupType::Heal)); // Because Heal is the last enum value, we can use it to get the range of powerup types

    Powerup powerup{};
    powerup.type = static_cast<PowerupType>(typeIndex);
    
    Position position{};
    position.value = {
       static_cast<float>(
           GetRandomValue(50, GetScreenWidth() - 50)
       ),
       static_cast<float>(
           GetRandomValue(50, GetScreenHeight() - 50)
       )
    };

    world.powerups.push_back(powerup);
    world.powerupPositions.push_back(position);
}

void SpawnSystem::Reset()
{
    m_spawnTimer = 1.0f;
    m_powerupTimer = 5.0f;
}
