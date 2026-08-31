#include "PowerupSystem.h"
#include "Player.h"
#include "raymath.h"

void PowerupSystem::Update(GameWorld& world, Player& player, const PowerupDatabase& database, float deltaTime)
{
    UpdateActivePowerups(world, deltaTime);
    CheckPickups(world, player, database);
}

void PowerupSystem::UpdateActivePowerups(GameWorld& world, float deltaTime)
{
    for (size_t i = 0; i < world.playerPowerups.size();)
    {
        ActivePowerup& powerup = world.playerPowerups[i];

        powerup.remainingTime -= deltaTime;

        if (powerup.remainingTime <= 0.f)
        {
            world.playerPowerups.erase(world.playerPowerups.begin() + i);

        }
        else
        {
            ++i;
        }
    }
}

void PowerupSystem::CheckPickups(GameWorld& world, Player& player, const PowerupDatabase& database)
{
    const Vector2 playerPosition = player.GetPosition();

    constexpr float pickupRadius = 10.f;

    for (size_t i = 0; i < world.powerups.size();)
    {
        const Vector2 powerupPosition = world.powerupPositions[i].value;
        const float distance = Vector2Distance(playerPosition, powerupPosition);

        if (distance <= pickupRadius)
        {
            const PowerupType type = world.powerups[i].type;

            const PowerupDefinition& definition = database.Get(type);

            ApplyPowerup(world, player, type, definition);

            world.powerups.erase(world.powerups.begin() + i);
            world.powerupPositions.erase(world.powerupPositions.begin() + i);
        }
        else
        {
            ++i;
        }
    }
}

void PowerupSystem::ApplyPowerup(GameWorld& world, Player& player, PowerupType type, const PowerupDefinition& definition)
{
    if (type == PowerupType::Heal)
    {
        player.Heal(definition.value);
        return;
    }

    ActivePowerup active{};

    active.type = type;
    active.remainingTime = definition.duration;
    active.value = definition.value;
    world.playerPowerups.push_back(active);
}


