#include "GameTimer.h"

GameTimer::GameTimer(float durationSeconds)
    : m_duration(durationSeconds)
    , m_remainingTime(durationSeconds)
{

}

void GameTimer::Reset()
{
    m_remainingTime = m_duration;
}

void GameTimer::Update(float deltaTime)
{
    if (m_remainingTime > 0.0f)
    {
        m_remainingTime -= deltaTime;
        if (m_remainingTime < 0.0f)
        {
            m_remainingTime = 0.0f;
        }
    }
}

float GameTimer::GetRemainingTime() const
{
    return m_remainingTime;
}

bool GameTimer::IsFinished() const
{
    return m_remainingTime <= 0.0f;
}

bool GameTimer::CheckForCutsceneTime(float timeToTrigger)
{   
    //so if deltaTime is less than or equal to timeToTrigger and isTriggered is false, we will trigger the cutscene
    if (m_remainingTime <= timeToTrigger && !isCutsceneTriggered)
    {   
        isCutsceneTriggered = true;
        return true;
    }
    return false;
}
