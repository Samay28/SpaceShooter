#include "Game.h"
#include "raylib.h" // Include raylib for timing functions and etc since only renderer is abstracted 

Game::Game() : m_renderer(1200, 720, "Space Shooter ")
, m_player(
    { 640.f,
    360.f })
{

}

void Game::Run()
{
    while (!m_renderer.ShouldClose())
    {
        const float deltaTime = GetFrameTime();

        Update(deltaTime);
        Render();
    }
}

void Game::Update(float deltaTime)
{
    m_player.Update(deltaTime);
}

void Game::Render()
{
    m_renderer.BeginFrame(); // Start drawing
    m_player.Render(); // Render the player
    m_renderer.EndFrame(); // Finish drawing 
}