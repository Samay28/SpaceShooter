#pragma once

#include "Core/GameWorld.h"
#include "Data/PowerupDatabase.h"

class Player;

class PowerupSystem
{
public:
    
    PowerupSystem();
    void Update(GameWorld& world, Player& player, const PowerupDatabase& database, float deltaTime);
    void ApplyAllPowerups(GameWorld& world, Player& player, const PowerupDatabase& database);
private:


    void UpdateActivePowerups(GameWorld& world, float deltaTime);
    void CheckPickups(GameWorld& world, Player& player, const PowerupDatabase& database);
    void ApplyPowerup(GameWorld& world, Player& player, PowerupType type, const PowerupDefinition& definition);

    

    Sound powerupPickupSound;
};