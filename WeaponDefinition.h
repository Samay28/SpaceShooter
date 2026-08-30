#pragma once

#include <cstdint>

enum class WeaponType : uint8_t
{
    Single,
    Double,
    Fast
};

struct WeaponDefinition
{
    float damage;
    float fireRate;
    float projectileSpeed;

    uint8_t projectileCount; // Number of projectiles fired per shot
    float projectileSpread; // Spread angle in degrees for multiple projectiles
};