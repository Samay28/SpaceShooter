#pragma once
#include "raylib.h"
#include <string>
#include <Data/EnemyDefinition.h>

class Player;
class GameWorld;
class Profiler;

struct DialogueNode;

enum class DialogueDisplayMode
{
    Choices, //Node 0 : Accept / Reject
    Continue, //Node 1 or Node 2 : Press Space
    TimedMessage //Node 3 or Node 4 : fade out
};

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
    void DrawBackground();

    void DrawCutsceneImposter(EnemyType type, Vector2 position, Vector2 playerPosition);
    void DrawCutsceneUI(const DialogueNode& node, DialogueDisplayMode mode, float secondsRemaining = 3.f);
    static Rectangle GetChoiceButtonRect(int choiceIndex);

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

    Texture2D m_backgroundTexture;

    void DrawWrappedText(
        const std::string& text,
        int x,
        int y,
        int maxWidth,
        int fontSize,
        int lineSpacing,
        Color color
    );

    void DrawChoiceButton(
        Rectangle rect,
        const std::string& label,
        Color backgroundColor,
        float alpha
    );
};