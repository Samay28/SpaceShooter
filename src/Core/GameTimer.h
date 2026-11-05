#pragma once

class GameTimer
{
public:
    explicit GameTimer(float durationSeconds);

    void Reset();
    void Update(float deltaTime);
    float GetRemainingTime() const; 
    bool IsFinished() const;

    //for cutscene trigger
    bool CheckForCutsceneTime(float timeToTrigger);
    void SetCutsceneTriggered(bool triggered) { isCutsceneTriggered = triggered; }
    void ResetCutsceneForRestart() { isCutsceneTriggered = false; }

private:
    float m_duration;
    float m_remainingTime;
    bool isCutsceneTriggered = false;
};
