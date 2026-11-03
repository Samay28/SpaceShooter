#pragma once

#include "Core/GameWorld.h"
#include "Data/EnemyDatabase.h"

#include "Data/PowerupDatabase.h"

class SpawnSystem
{
public:
    void Update(GameWorld& world, const EnemyDatabase& enemyDatabase , float deltaTime);
    void Reset();
    void SpawnImposter(GameWorld& world, const EnemyDatabase& enemyDatabase, class GameBranchController& gameBranch);
private:
    float m_spawnTimer = 0.0f;
    float m_spawnInterval = 1.0f;

    float m_powerupTimer = 5.f;
    float m_powerupInterval = 5.f;

    float m_maxEnemies = 10.f;

    void SpawnEnemy(GameWorld& world, const EnemyDatabase& enemyDatabase);
    void SpawnPowerup(GameWorld& world); // no need for database here, as we can randomly choose a enum

    
    bool spawned = false; // Flag to track if the imposter has been spawned
};