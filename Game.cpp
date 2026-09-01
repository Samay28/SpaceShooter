#include "Game.h"
#include "raylib.h" // Include raylib for timing functions and etc since only renderer is abstracted 

Game::Game()
    : m_renderer(1200, 720, "Space Shooter")
    , m_player({ 640.0f, 600.0f })
    , m_projectileSpeed(500.0f)
{
    if (!m_enemyDatabase.Load("assets/enemies.txt"))
    {
        TraceLog(
            LOG_ERROR,
            "Failed to load enemies.txt"
        );
    }

    if (!m_weaponDatabase.Load("assets/weapons.txt"))
    {
        TraceLog(
            LOG_ERROR,
            "Failed to load weapons.txt"
        );
    }
    if(!m_powerupDatabse.Load("assets/powerups.txt"))
    {
        TraceLog(
            LOG_ERROR,
            "Failed to load powerups.txt"
        );
    }
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

    //Game state check
    if (m_gameState == GameState::GameOver)
    {
        if (IsKeyPressed(KEY_R))
        {
            RestartGame();
        }
        return;
    }

    //---------------- Powerup System ----------------
    m_powerupSystem.Update(
        m_world,
        m_player,
        m_powerupDatabse,
        deltaTime);


    // ---------------- Player ----------------
    float speedMultiplier = 1.0f;
    for(const ActivePowerup& powerup : m_world.playerPowerups)
    {
        if (powerup.type == PowerupType::SpeedBoost)
        {
            speedMultiplier *= powerup.value;
        }
    }

    m_player.Update(deltaTime, speedMultiplier);
    
    //  we need to check if the rapid fire powerup is active and set the shoot interval accordingly
    bool rapidFireActive = false;

    for (const ActivePowerup& powerup :
        m_world.playerPowerups)
    {
        if (powerup.type ==
            PowerupType::RapidFire)
        {
            rapidFireActive = true;
            break;
        }
    }

    if (rapidFireActive)
    {
        m_player.SetShootInterval(
            0.5f / 2.0f
        );
    }
    else
    {
        m_player.SetShootInterval(
            0.5f
        );
    }

    // ---------------- Enemy Spawning ----------------

    m_spawnSystem.Update(
        m_world,
        m_enemyDatabase,
        deltaTime);

    // ---------------- Enemy AI ----------------

    m_aiSystem.Update(
        m_world,
        m_enemyDatabase,
        m_player.GetPosition(),
        deltaTime);

    m_weaponSystem.Update(
        m_world,
        m_player,
        m_enemyDatabase,
        m_weaponDatabase,
        m_player.GetPosition(),
        deltaTime
    );

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
        m_world,m_player, m_enemyDatabase);

    if(m_player.GetHealth() <= 0.0f)
    {
        m_gameState = GameState::GameOver;
    }

    // ---------------- Cleanup ----------------

    CleanupProjectiles(m_world);

    CleanupEnemies(m_world);
}

void Game::Render()
{
    m_renderer.BeginFrame(); // Start drawing
    m_player.Render(); // Render the player
    m_renderer.Render(m_world); // Render the projectiles
    m_renderer.DrawHUD(m_player, m_world); // Render the HUD
    if(m_gameState == GameState::GameOver)
    {
        m_renderer.DrawGameOver(); // Render the game over screen
    }
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

void Game::RestartGame()
{
    // Reset the game state
    m_gameState = GameState::Playing;

    //player reset and spawner
    m_player.Reset();
    m_spawnSystem.Reset();

    // Clear runtime entities
    m_world.enemies.clear();
    m_world.enemyHealth.clear();
    m_world.enemyPositions.clear();
    m_world.enemyVelocities.clear();

    m_world.projectiles.clear();
    m_world.projectilePositions.clear();
    m_world.projectileVelocities.clear();

    m_world.powerups.clear();
    m_world.powerupPositions.clear();

    m_world.playerPowerups.clear();


}




