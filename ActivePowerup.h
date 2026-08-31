#pragma once
#include "Powerup.h"

struct ActivePowerup
{
    PowerupType type;
    float remainingTime; 
    float value; // The value of the powerup, e.g., speed multiplier, shield strength, etc.
};
