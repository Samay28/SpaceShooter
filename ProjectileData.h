#pragma once

#include <stdint.h>

using EntityID = uint32_t; // Define EntityID as a 32-bit unsigned integer

struct Projectile
{
    EntityID owner;
    float damage;
    float lifetime;
};
