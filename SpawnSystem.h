#pragma once

#include "GameWorld.h"
#include "EnemyDatabase.h"

class SpawnSystem
{
public:
    void Update(GameWorld& world, const EnemyDatabase& enemyDatabase , float deltaTime);

private:
    float m_spawnTimer = 0.0f;
    float m_spawnInterval = 1.0f;

    void SpawnEnemy(GameWorld& world, const EnemyDatabase& enemyDatabase);
};