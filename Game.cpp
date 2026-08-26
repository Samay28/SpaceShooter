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
    m_movementSystem.Update(m_world, deltaTime);
    m_projectileSystem.Update(m_world, deltaTime);
    CleanupProjectiles(m_world);
}

void Game::CleanupProjectiles(GameWorld& world)
{
    for (size_t i = 0; i < world.projectiles.size();)
    {
        if (world.projectiles[i].lifetime <= 0.0f)
        {
            // For now, remove the corresponding data.
            world.projectiles.erase(
                world.projectiles.begin() + i);

            world.positions.erase(
                world.positions.begin() + i);

            world.velocities.erase(
                world.velocities.begin() + i);
        }
        else
        {
            ++i;
        }
    }
}
void Game::Render()
{
    m_renderer.BeginFrame(); // Start drawing
    m_player.Render(); // Render the player
    m_renderer.Render(m_world); // Render the projectiles
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

    // Create a new projectile entity
    Projectile projectile{};
    projectile.owner = 0; // player has ID 0
    projectile.damage = 10.0f; // Example damage value
    projectile.lifetime = 2.0f; // Example lifetime value

    // Add the projectile to the world
    m_world.projectiles.push_back(projectile);
    m_world.velocities.push_back(
        Velocity{ velocity }
    );
    m_world.positions.push_back(Position{ spawnPosition });

    m_player.Fire();
}


