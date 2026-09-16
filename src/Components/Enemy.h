#pragma once

#include <cstdint>
#include "Data/EnemyDefinition.h"


enum class EnemyState : uint8_t
{
    Moving,
    Attacking,
};

struct Enemy
{   

    EnemyType type; 

    EnemyState state;

    float stateTimer; 

    Vector2 targetPosition;

    float rotation = 0.f; // Angle in degrees for sprite rotation
    float weaponCooldown;
};

