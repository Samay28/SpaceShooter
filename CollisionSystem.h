#pragma once

#include "GameWorld.h"
#include "Player.h"

#include "EnemyDatabase.h"

class CollisionSystem
{
public:
    void Update(GameWorld& world, Player& player, const EnemyDatabase& enemyDatabase);
};
