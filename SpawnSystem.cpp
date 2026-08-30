#include "SpawnSystem.h"
#include "raymath.h"

void SpawnSystem::Update(GameWorld& world, const EnemyDatabase& enemyDatabase, float deltaTime)
{
    m_spawnTimer -= deltaTime;

    if(m_spawnTimer <= 0.0f)
    {
        SpawnEnemy(world, enemyDatabase);
        m_spawnTimer = m_spawnInterval;
    }
}

void SpawnSystem::SpawnEnemy(GameWorld& world, const EnemyDatabase& enemyDatabase)
{   
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
