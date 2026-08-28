#pragma once

#include "GameWorld.h"

class SpawnSystem
{
public:
    void Update(GameWorld& world, float deltaTime);

private:
    float m_spawnTimer = 0.0f;
    float m_spawnInterval = 1.0f;

    void SpawnEnemy(GameWorld& world);
};