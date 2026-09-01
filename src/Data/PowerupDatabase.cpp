#include "PowerupDatabase.h"
#include "DataParser.h"

namespace
{
    PowerupType ParsePowerupType(
        const std::string& name)
    {
        if (name == "DoubleShot")
            return PowerupType::DoubleShot;

        if (name == "TripleShot")
            return PowerupType::TripleShot;

        if (name == "RapidFire")
            return PowerupType::RapidFire;

        if (name == "SpeedBoost")
            return PowerupType::SpeedBoost;

        if (name == "Shield")
            return PowerupType::Shield;

        return PowerupType::Heal;
    }

    size_t ToIndex(PowerupType type)
    {
        return static_cast<size_t>(type);
    }
}

bool PowerupDatabase::Load(
    const char* filePath)
{
    const auto sections =
        DataParser::Load(filePath);

    bool loadedAnything = false;

    for (const DataSection& section : sections)
    {
        const PowerupType type =
            ParsePowerupType(section.name);

        PowerupDefinition definition{};

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

        definition.type = type;

        definition.duration =
            getFloat("Duration", 0.0f);

        definition.value =
            getFloat("Value", 0.0f);

        m_definitions[ToIndex(type)] =
            definition;

        loadedAnything = true;
    }

    return loadedAnything;
}

const PowerupDefinition&
PowerupDatabase::Get(PowerupType type) const
{
    return m_definitions[ToIndex(type)];
}
