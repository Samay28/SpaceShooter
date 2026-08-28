#include "Renderer.h"
#include "GameWorld.h"
#include "raylib.h"

Renderer::Renderer(
    int screenWidth,
    int screenHeight,
    const char* title)
    : m_screenWidth(screenWidth)
    , m_screenHeight(screenHeight)
{
    InitWindow(
        m_screenWidth,
        m_screenHeight,
        title
    );

    SetTargetFPS(60);
}

Renderer::~Renderer()
{
    CloseWindow();
}

void Renderer::BeginFrame()
{
    BeginDrawing();

    ClearBackground(BLACK);
}

void Renderer::EndFrame()
{
    EndDrawing();
}

bool Renderer::ShouldClose() const
{
    return WindowShouldClose();
}

void Renderer::Render(const GameWorld& world)
{   
    // Render projectiles
    for (size_t i = 0; i < world.projectiles.size(); ++i)
    {
        const Vector2 pos = world.projectilePositions[i].value;
        DrawCircleV(pos, 4.0f, YELLOW);
    }

    // Render enemies
    for(size_t i=0; i<world.enemies.size(); ++i)
    {
        DrawCircleV(
            world.enemyPositions[i].value,
            world.enemies[i].radius,
            RED);
    }
}
