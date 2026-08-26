#include "GameWorld.h"

EntityID GameWorld::CreateEntity()
{
    return m_nextEntityID++;
}