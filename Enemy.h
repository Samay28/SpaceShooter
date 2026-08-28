#pragma once

#include <cstdint>

enum class EnemyType : uint8_t
{
    Basic,
    duo,
    Fast,
    Tank
};

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

    float attackDuration;

    float moveSpeed;

    float health;

    float radius;
};

