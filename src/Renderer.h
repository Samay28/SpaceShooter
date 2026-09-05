#pragma once

class Player;
class GameWorld;
class Profiler;

class Renderer
{
public:
    Renderer(int screenWidth, int screenHeight, const char* title);
    ~Renderer();

    void BeginFrame();
    void EndFrame();

    bool ShouldClose() const;

    void Render(const GameWorld& world);
    void DrawPowerupUI(const GameWorld& world);

    void DrawScorePopups(const GameWorld& world);
    void DrawHUD(const Player& player, const GameWorld& world, float remainingTime);
    void DrawHealthBar(const Player& player);
    void DrawGameOver(const GameWorld& world);

    void DrawProfiler(const Profiler& profiler);

private:
    int m_screenWidth;
    int m_screenHeight;
};