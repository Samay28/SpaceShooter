#pragma once

#include <stdint.h>

using EntityID = uint32_t; // Define EntityID as a 32-bit unsigned integer

enum class ProjectileOwner : uint8_t
{
    Player,
    Enemy
};

struct Projectile
{
    ProjectileOwner owner;
    float damage;
    float lifetime;
};
