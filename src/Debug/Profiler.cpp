#include "Profiler.h"
#include <algorithm>
#include <numeric>

void Profiler::BeginFrame()
{
    m_frameStart = Clock::now();
}

void Profiler::EndFrame()
{
    const TimePoint frameEnd = Clock::now();

    m_frameTime = std::chrono::duration<float, std::milli>(frameEnd - m_frameStart).count();
}

void Profiler::Begin(const std::string& name)
{
    ProfileEntry& entry = m_entries[name];
    // Set the start time for the profiling entry
    entry.startTime = Clock::now();
}

void Profiler::End(const std::string& name)
{
    const TimePoint endTime = Clock::now();
    const auto it = m_entries.find(name);

    if(it == m_entries.end())
    {
        // Entry not found, return early
        return;
    }

    ProfileEntry& entry = it->second;
    // Update the current time for the profiling entry
    entry.currentMiliseconds = std::chrono::duration<float, std::milli>(endTime - entry.startTime).count();

    // Update the statistics for the profiling entry
    entry.samples.push_back(entry.currentMiliseconds);

    if(entry.samples.size() > MaxSamples)
    {   

        // Remove the oldest sample to maintain the maximum number of samples
        entry.samples.erase(entry.samples.begin());
    }
    UpdateStatistics(entry);
}

float Profiler::GetFrameTime() const
{
    return m_frameTime;
}

float Profiler::GetCurrentTime(const std::string& name) const
{
    const auto it = m_entries.find(name);
    if(it != m_entries.end())
    {
        return it->second.currentMiliseconds;
    }
    return 0.0f;
}

float Profiler::GetAverageTime(const std::string& name) const
{
    const auto it = m_entries.find(name);
    if(it != m_entries.end())
    {
        return it->second.averageMiliseconds;
    }
    return 0.0f;
}

float Profiler::GetMaxTime(const std::string& name) const
{
    const auto it = m_entries.find(name);
    if (it != m_entries.end())
    {
        return it->second.maxMiliseconds;
    }
    return 0.0f;
}

bool Profiler::HasEntry(const std::string& name) const
{
    return m_entries.find(name)!=m_entries.end();
}

void Profiler::UpdateStatistics(ProfileEntry& entry)
{   
    //if there are no samples, we can't calculate average or max, so we set them to 0
    if (entry.samples.empty())
    {
        entry.averageMiliseconds = 0.f;
        entry.maxMiliseconds = 0.f;
        return;
    }

    // Calculate the maximum time from the samples
    const float total = std::accumulate(entry.samples.begin(), entry.samples.end(), 0.f);

    entry.averageMiliseconds = total / static_cast<float>(entry.samples.size());

    entry.maxMiliseconds = *std::max_element(entry.samples.begin(), entry.samples.end());
}
