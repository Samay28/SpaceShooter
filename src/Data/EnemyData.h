#pragma once
#include <cstdint>
#include <raymath.h>

using EnemyType = uint8_t;

struct Enemy
{
    EnemyType type;
    float stateTimer;
};