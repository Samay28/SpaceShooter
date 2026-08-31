#pragma once

#include "GameWorld.h"
#include "WeaponDatabase.h"
#include "EnemyDatabase.h"

class Player;

class WeaponSystem
{
public:
    void Update(GameWorld& world, Player& player, const EnemyDatabase& enemyDatabase, const WeaponDatabase& weaponDatabase, Vector2 playerPos ,float deltaTime);

private:
    bool HasPowerup(const GameWorld& world, PowerupType type) const;

    float GetPowerupValue(
        const GameWorld& world,
        PowerupType type,
        float defaultValue) const;

    void FirePlayerWeapon(
        GameWorld& world,
        Player& player,
        const WeaponDefinition& weapon);

};