#include "GameWorld.h"

EntityID GameWorld::CreateEntity()
{
    return m_nextEntityID++;
}

void GameWorld::ClearAll()
{
    enemies.clear();
    enemyHealth.clear();
    enemyPositions.clear();
    enemyVelocities.clear();

    projectiles.clear();
    projectilePositions.clear();
    projectileVelocities.clear();

    powerups.clear();
    powerupPositions.clear();

    //clear ally now
    allies.clear();
    allyHealth.clear();
    allyPositions.clear();
}
