#pragma once

#include "Entity.h"
#include "Components/Position.h"
#include "Components/Velocity.h"   
#include "Components/Enemy.h"
#include "Components/Health.h"
#include "Data/ProjectileData.h"

#include "Components/Powerup.h"
#include "Components/ActivePowerup.h"

#include <vector>

class GameWorld
{
public:
    EntityID CreateEntity(); 

    //---------Player----------------
    EntityID playerEntity = INVALID_ENTITY_ID;
    std::vector<ActivePowerup> playerPowerups;


    //---------Projectiles----------------
    std::vector<Position> projectilePositions;
    std::vector<Velocity> projectileVelocities;
    std::vector<Projectile> projectiles;

    //---------Enemies----------------
    std::vector<Position> enemyPositions;
    std::vector<Velocity> enemyVelocities;
    std::vector<Enemy> enemies;
    std::vector<Health> enemyHealth;

    //---------Powerups----------------
    std::vector<Position> powerupPositions;
    std::vector<Powerup> powerups;

private:
    EntityID m_nextEntityID = 1; // Start from 1 to avoid using INVALID_ENTITY_ID
};