#pragma once

#include <cstdint>
#include "EnemyDefinition.h"


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

    
    float weaponCooldown;
};

