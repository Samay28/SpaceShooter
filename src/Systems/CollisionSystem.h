#pragma once

#include "Core/GameWorld.h"
#include "Player.h"

#include "Data/EnemyDatabase.h"

class CollisionSystem
{
public:
    void Update(GameWorld& world, Player& player, const EnemyDatabase& enemyDatabase);

    //helpers
    bool HasShield(const GameWorld& world) const;
    void RemoveShield(GameWorld& world);
};
