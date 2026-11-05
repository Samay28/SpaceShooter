#pragma once
#include "Core/GameWorld.h"
#include "Data/EnemyDatabase.h"

class AISystem
{
public:

    // Update the AI system, which includes enemy behavior and decision-making.
    void Update(
        GameWorld& world,
        const EnemyDatabase& enemyDatabase,
        Vector2 playerPosition,
        float deltaTime);
    static bool AllyChooseEnemyToAttack(const GameWorld& world, size_t& outEnemyIndex);
private:
    void ChooseNewTarget(
        GameWorld& world,
        size_t index);

   

    float RandomAttackDuration(
        const EnemyDefinition& definition);
};