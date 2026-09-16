#pragma once
#include "raylib.h"

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

    void DrawProfiler(const Profiler& profiler, float fixedDeltaTime, int simulationSteps);

private:
    int m_screenWidth;
    int m_screenHeight;


private:
    Texture2D m_basicEnemyTexture;
    Texture2D m_duoEnemyTexture;
    Texture2D m_fastEnemyTexture;
    Texture2D m_tankEnemyTexture;

    //powerup textures
    Texture2D m_doubleShotTexture;
    Texture2D m_tripleShotTexture;
    Texture2D m_rapidFireTexture;
    Texture2D m_speedBoostTexture;
    Texture2D m_shieldTexture;
    Texture2D m_healthPackTexture;
};