#pragma once

class GameTimer
{
public:
    explicit GameTimer(float durationSeconds);

    void Reset();
    void Update(float deltaTime);
    float GetRemainingTime() const; 
    bool IsFinished() const;

private:
    float m_duration;
    float m_remainingTime;
};
