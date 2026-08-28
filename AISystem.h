#pragma once
#include "GameWorld.h"

class AISystem
{
public:

    // Update the AI system, which includes enemy behavior and decision-making.
    void Update(
        GameWorld& world,
        Vector2 playerPosition,
        float deltaTime);

private:
    void UpdateEnemy(
        GameWorld& world,
        size_t index,
        Vector2 playerPosition,
        float deltaTime);
};