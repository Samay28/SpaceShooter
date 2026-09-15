#pragma once

#include "Core/GameWorld.h"
#include "Player.h"

#include "Data/EnemyDatabase.h"

class CollisionSystem
{
public:

    CollisionSystem();
    ~CollisionSystem();

    void Update(GameWorld& world, Player& player, const EnemyDatabase& enemyDatabase, const class SpatialGrid& spatialGrid);

    //helpers
    bool HasShield(const GameWorld& world) const;
    void RemoveShield(GameWorld& world);

    Sound playerHitSound;
    Sound shieldBreakSound;
};
