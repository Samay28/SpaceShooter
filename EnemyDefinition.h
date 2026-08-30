#pragma once
#include <cstdint> 
//uint is an unsigned integer type that can hold only non-negative values. 
// It is often used when you want to ensure that a variable cannot be negative, such as for counts, 
// sizes, or indices. The size of uint can vary depending on the platform and compiler, 
// but it is typically 32 bits (4 bytes) on most modern systems.


#include "WeaponDefinition.h"

enum class EnemyType : uint8_t
{
    Basic,
    Duo,
    Fast,
    Tank
};

//The definition contains the stuff that distinguishes enemies.
struct EnemyDefinition
{
    float maxHealth;
    float moveSpeed;
    float radius;

    float attackDurationMin;
    float attackDurationMax;

    WeaponType weaponType;
};
