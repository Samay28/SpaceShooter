#pragma once

#include "Entity.h"
#include "Position.h"
#include "Velocity.h"   
#include "Enemy.h"
#include "Health.h"
#include "ProjectileData.h"

#include <vector>

class GameWorld
{
public:
    EntityID CreateEntity(); 

    //---------Projectiles----------------
    std::vector<Position> projectilePositions;
    std::vector<Velocity> projectileVelocities;
    std::vector<Projectile> projectiles;

    //---------Enemies----------------
    std::vector<Position> enemyPositions;
    std::vector<Velocity> enemyVelocities;
    std::vector<Enemy> enemies;
    std::vector<Health> enemyHealth;

private:
    EntityID m_nextEntityID = 1; // Start from 1 to avoid using INVALID_ENTITY_ID
};