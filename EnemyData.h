#pragma once
#include <cstdint>

using EnemyType = uint8_t;

struct Enemy
{
    EnemyType type;
    float stateTimer;
};