#pragma once
#include "Core/GameWorld.h"
#include "Data/EnemyDatabase.h"

class ScoreSystem
{
public:
    void Update(GameWorld& world, float deltaTime);

    void RegisterEnemyDeath(GameWorld& world, const Enemy& enemy, Vector2 enemyPos, const EnemyDatabase& enemyDatabase);

    void LoadHighscore(GameWorld& world);
    void SaveHighscore(GameWorld& world);

private:
    static constexpr float PopupDuration = 1.0f; // Duration for which the score popup is visible
};
