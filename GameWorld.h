#pragma once

#include "Entity.h"
#include "Position.h"
#include "Velocity.h"   
#include "ProjectileData.h"

#include <vector>

class GameWorld
{
public:
    EntityID CreateEntity(); 

    //runtime data container for the game world
    std::vector<Position> positions;
    std::vector<Velocity> velocities;
    std::vector<Projectile> projectiles;

private:
    EntityID m_nextEntityID = 1; // Start from 1 to avoid using INVALID_ENTITY_ID
};