#include "ScoreSystem.h"
#include <fstream>

void ScoreSystem::Update(GameWorld& world, float deltaTime)
{
    for (size_t i = 0; i < world.scorePopup.size(); ++i)
    {
        ScorePopup& popup = world.scorePopup[i];
        popup.remainingTime -= deltaTime;

        if(popup.remainingTime <= 0.0f)
        {
            world.score += popup.score; // Add the score value to the total score
            world.scorePopup.erase(world.scorePopup.begin() + i);
        }
        else
        {
            ++i;
        }
    }
}

void ScoreSystem::RegisterEnemyDeath(GameWorld& world, const Enemy& enemy, Vector2 enemyPos, const EnemyDatabase& enemyDatabase)
{
    const EnemyDefinition& definition = enemyDatabase.Get(enemy.type);  

    ScorePopup popup{};
    popup.position = enemyPos;
    popup.score = definition.score; 
    popup.remainingTime = PopupDuration;
    world.scorePopup.push_back(popup);
}

void ScoreSystem::LoadHighscore(GameWorld& world)
{
    std::ifstream file("save/highscore.txt");
    if(!file.is_open())
    {
        world.highScore = 0;
        return;
    }
    file >> world.highScore;
}

void ScoreSystem::SaveHighscore(GameWorld& world)
{
    world.highScore = world.score;

    std::ofstream file(
        "save/highscore.txt"
    );

    if (!file.is_open())
    {
        return;
    }

    file << world.score;
}
