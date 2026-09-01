#pragma once
#include "PowerDefinition.h"
#include "Components/Powerup.h"

class PowerupDatabase
{
public:
    bool Load(const char* filePath);

    const PowerupDefinition& Get(PowerupType type) const;

private:
    PowerupDefinition m_definitions[6]; // Assuming 6 powerup types
};