#pragma once
#include "WeaponDefinition.h"

class WeaponDatabase
{
public:
    bool Load(const char* filePath);

    // Get the weapon definition for a given weapon type
    const WeaponDefinition& Get(WeaponType type) const;
private:
    WeaponDefinition m_definitions[3];
};
