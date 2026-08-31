#pragma once

#include "GameWorld.h"
#include "PowerupDatabase.h"

class Player;

class PowerupSystem
{
public:
    void Update(GameWorld& world, Player& player, const PowerupDatabase& database, float deltaTime);

private:

    void UpdateActivePowerups(GameWorld& world, float deltaTime);
    void CheckPickups(GameWorld& world, Player& player, const PowerupDatabase& database);
    void ApplyPowerup(GameWorld& world, Player& player, PowerupType type, const PowerupDefinition& definition);
};