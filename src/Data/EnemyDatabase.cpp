#include "EnemyDatabase.h"
#include "DataParser.h"

namespace
{
    EnemyType ParseEnemyType(
        const std::string& name)
    {
        if (name == "Basic")
            return EnemyType::Basic;

        if (name == "Duo")
            return EnemyType::Duo;

        if (name == "Fast")
            return EnemyType::Fast;

        return EnemyType::Tank;
    }

    WeaponType ParseWeaponType(
        const std::string& name)
    {
        if (name == "Double")
            return WeaponType::Double;

        if (name == "Fast")
            return WeaponType::Fast;

        return WeaponType::Single;
    }

    size_t ToIndex(EnemyType type)
    {
        return static_cast<size_t>(type);
    }
}

bool EnemyDatabase::Load(
    const char* filePath)
{
    const auto sections =
        DataParser::Load(filePath);

    bool loadedAnything = false;

    for (const DataSection& section : sections)
    {
        const EnemyType type =
            ParseEnemyType(section.name);

        EnemyDefinition definition{};

        const auto getFloat =
            [&](const char* key, float defaultValue)
            {
                const auto it =
                    section.values.find(key);

                if (it == section.values.end())
                {
                    return defaultValue;
                }

                return std::stof(it->second);
            };

        const auto getString =
            [&](const char* key,
                const char* defaultValue)
            {
                const auto it =
                    section.values.find(key);

                if (it == section.values.end())
                {
                    return std::string(defaultValue);
                }

                return it->second;
            };

        definition.maxHealth =
            getFloat("Health", 50.0f);

        definition.moveSpeed =
            getFloat("Speed", 80.0f);

        definition.radius =
            getFloat("Radius", 15.0f);

        definition.attackDurationMin =
            getFloat("AttackDurationMin", 1.5f);

        definition.attackDurationMax =
            getFloat("AttackDurationMax", 3.0f);

        definition.score =
            getFloat("Score", 100);

        definition.weaponType =
            ParseWeaponType(
                getString(
                    "Weapon",
                    "Single"
                )
            );

        m_definitions[ToIndex(type)] =
            definition;

        loadedAnything = true;
    }

    return loadedAnything;
}

const EnemyDefinition&
EnemyDatabase::Get(EnemyType type) const
{
    return m_definitions[ToIndex(type)];
}