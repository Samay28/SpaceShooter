#pragma once

#include "raylib.h"

class GameWorld;

class Renderer
{
public:
    Renderer(int screenWidth, int screenHeight, const char* title);
    ~Renderer();

    void BeginFrame();
    void Render(const GameWorld& world);
    void EndFrame();

    bool ShouldClose() const;

    int GetScreenWidth() const;
    int GetScreenHeight() const;

private:
    int m_screenWidth;
    int m_screenHeight;

    void DrawWorld(const GameWorld& world);
    void DrawPlayer(const GameWorld& world);
    void DrawEnemies(const GameWorld& world);
    void DrawProjectiles(const GameWorld& world);
    void DrawPowerups(const GameWorld& world);
    void DrawUI(const GameWorld& world);
};