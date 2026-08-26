#include "ProjectileSystem.h"

void ProjectileSystem::Update(GameWorld& world, float DeltaTime)
{
    for (size_t i = 0; i < world.projectiles.size(); ++i)
    {
        //decrease the lifetime of the projectile
        world.projectiles[i].lifetime -= DeltaTime;
    }
}
