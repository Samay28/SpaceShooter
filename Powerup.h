#pragma once

#include <cstdint>

enum class PowerupType : uint8_t
{
    DoubleShot,
    TripleShot,
    RapidFire,
    SpeedBoost,
    Shield,
    Heal
};

struct Powerup
{
    PowerupType type;
};