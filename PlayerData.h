#pragma once
#include <stdint.h>

using EntityID = uint32_t; // Define EntityID as a 32-bit unsigned integer

struct Player
{
    EntityID entity;

    float moveSpeed;
    float shootCooldown;
    float shootInterval;
};
