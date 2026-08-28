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
    // ---------------- Player ----------------

    m_player.Update(deltaTime);

    HandlePlayerShooting();

    // ---------------- Enemy Spawning ----------------

    m_spawnSystem.Update(
        m_world,
        deltaTime);

    // ---------------- Enemy AI ----------------

    m_aiSystem.Update(
        m_world,
        m_player.GetPosition(),
        deltaTime);

    // ---------------- Movement ----------------

    m_movementSystem.Update(
        m_world,
        deltaTime);

    // ---------------- Projectiles ----------------

    m_projectileSystem.Update(
        m_world,
        deltaTime);

    // ---------------- Collision ----------------

    m_collisionSystem.Update(
        m_world);

    // ---------------- Cleanup ----------------

    CleanupProjectiles(m_world);

    CleanupEnemies(m_world);
}

void Game::Render()
{
    m_renderer.BeginFrame(); // Start drawing
    m_player.Render(); // Render the player
    m_renderer.Render(m_world); // Render the projectiles
    m_renderer.EndFrame(); // Finish drawing 
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

            world.projectilePositions.erase(
                world.projectilePositions.begin() + i);

            world.projectileVelocities.erase(
                world.projectileVelocities.begin() + i);
        }
        else
        {
            ++i;
        }
    }
}

void Game::CleanupEnemies(GameWorld& world)
{
    for (size_t i = 0; i < world.enemies.size();)
    {
        if(world.enemyHealth[i].currentHealth <= 0.0f)
        {
           
            world.enemies.erase(
                world.enemies.begin() + i);
            world.enemyHealth.erase(
                world.enemyHealth.begin() + i);
            world.enemyPositions.erase(
                world.enemyPositions.begin() + i);
            world.enemyVelocities.erase(
                world.enemyVelocities.begin() + i);
        }
        else
        {
            ++i;
        }
    }
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
    projectile.damage = 50.0f; // Example damage value
    projectile.lifetime = 2.0f; // Example lifetime value

    // Add the projectile to the world
    m_world.projectiles.push_back(projectile);
    m_world.projectilePositions.push_back(
        Position{ spawnPosition });
    m_world.projectileVelocities.push_back(
        Velocity{ velocity });

    m_player.Fire();
}


