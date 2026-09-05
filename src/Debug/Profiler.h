#pragma once

#include <chrono>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

class Profiler
{
public:
    //high resolution clock for measuring time
    using Clock = std::chrono::high_resolution_clock;

    //time point is a point in time measured by the clock
    using TimePoint = Clock::time_point;

    struct ProfileEntry
    {
        TimePoint startTime;

        float currentMiliseconds = 0.f;
        float averageMiliseconds = 0.f;
        float maxMiliseconds = 0.f;

        std::vector<float> samples;
    };

    //Functions
    void BeginFrame();
    void EndFrame();

    //overloaded functions to begin and end profiling a specific code block
    void Begin(const std::string& name);
    void End(const std::string& name);

    float GetFrameTime() const;

    float GetCurrentTime(const std::string& name) const;
    float GetAverageTime(const std::string& name) const;
    float GetMaxTime(const std::string& name) const;

    bool HasEntry(const std::string& name) const;

private:
    static constexpr std::size_t MaxSamples = 120;
    void UpdateStatistics(ProfileEntry& entry);

private:
    TimePoint m_frameStart{};
    float m_frameTime = 0.f;

    //map to store profiling entries with their names as keys
    std::unordered_map<std::string, ProfileEntry> m_entries;
};
