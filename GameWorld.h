#pragma once

#include "Entity.h"
#include "Position.h"
#include "Velocity.h"   
#include "Enemy.h"
#include "Health.h"
#include "ProjectileData.h"

#include "Powerup.h"
#include "ActivePowerup.h"

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