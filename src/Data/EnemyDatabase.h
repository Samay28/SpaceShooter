#pragma once

#include "EnemyDefinition.h"

class EnemyDatabase
{
public:
    bool Load(const char* filePath);

    const EnemyDefinition&
        Get(EnemyType type) const;

private:
    EnemyDefinition m_definitions[4];
};