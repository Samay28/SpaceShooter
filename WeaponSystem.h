#pragma once

#include "GameWorld.h"
#include "WeaponDatabase.h"
#include "EnemyDatabase.h"

class WeaponSystem
{
public:
    void Update(GameWorld& world, const EnemyDatabase& enemyDatabase, const WeaponDatabase& weaponDatabase, Vector2 playerPos ,float deltaTime);
};