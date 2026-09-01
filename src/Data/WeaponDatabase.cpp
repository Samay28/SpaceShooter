#include "WeaponDatabase.h"
#include "DataParser.h"

#include <cstdlib>

namespace
{
    WeaponType ParseWeaponType(
        const std::string& name)
    {
        if (name == "Single")
            return WeaponType::Single;

        if (name == "Double")
            return WeaponType::Double;

        return WeaponType::Fast;
    }

    size_t ToIndex(WeaponType type)
    {
        return static_cast<size_t>(type);
    }
}

bool WeaponDatabase::Load(
    const char* filePath)
{
    const auto sections =
        DataParser::Load(filePath);

    bool loadedAnything = false;

    for (const DataSection& section : sections)
    {
        const WeaponType type =
            ParseWeaponType(section.name);

        WeaponDefinition definition{};

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

        const auto getInt =
            [&](const char* key, int defaultValue)
            {
                const auto it =
                    section.values.find(key);

                if (it == section.values.end())
                {
                    return defaultValue;
                }

                return std::stoi(it->second);
            };

        definition.damage =
            getFloat("Damage", 10.0f);

        definition.fireRate =
            getFloat("FireRate", 1.0f);

        definition.projectileSpeed =
            getFloat("ProjectileSpeed", 300.0f);

        definition.projectileCount =
            static_cast<uint8_t>(
                getInt("ProjectileCount", 1)
                );

        definition.projectileSpread =
            getFloat("ProjectileSpread", 0.0f);

        m_definitions[ToIndex(type)] =
            definition;

        loadedAnything = true;
    }

    return loadedAnything;
}

const WeaponDefinition&
WeaponDatabase::Get(WeaponType type) const
{
    return m_definitions[ToIndex(type)];
}