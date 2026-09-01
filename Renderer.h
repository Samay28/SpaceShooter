#pragma once

class Player;
class GameWorld;
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

    void DrawHUD(const Player& player, const GameWorld& world);
    void DrawHealthBar(const Player& player);
    void DrawGameOver();

private:
    int m_screenWidth;
    int m_screenHeight;
};