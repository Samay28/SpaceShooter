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

    //--------------Projectile Management----------------
    HandlePlayerShooting();
    UpdateProjectiles(deltaTime);
    RemoveDeadProjectiles();
}

void Game::Render()
{
    m_renderer.BeginFrame(); // Start drawing
    m_player.Render(); // Render the player
    RenderProjectile(); // Render all projectiles
    m_renderer.EndFrame(); // Finish drawing 
}

void Game::HandlePlayerShooting()
{
    if(!m_player.WantsToShoot())
    {
        return;
    }

    const Vector2 spawnPosition =
        m_player.GetProjectileSpawnPos();

    const Vector2 velocity = {
        0.0f,
        -m_projectileSpeed
    };

    //track the projectile in the vector of projectiles
    m_projectiles.emplace_back(
        spawnPosition,
        velocity
    );

    m_player.Fire();
}

void Game::UpdateProjectiles(float deltaTime)
{
    for(Projectile& p : m_projectiles)
    {
        p.Update(deltaTime);
    }
}

void Game::RenderProjectile()
{
    for (const Projectile& projectile : m_projectiles)
    {
        projectile.Render();
    }
}

void Game::RemoveDeadProjectiles()
{
    // Remove projectiles that are no longer alive
    m_projectiles.erase(
        std::remove_if(
            m_projectiles.begin(),
            m_projectiles.end(),
            [](const Projectile& projectile)
            {
                return !projectile.isAlive();
            }),
        m_projectiles.end()
    );
}
