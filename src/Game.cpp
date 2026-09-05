#include "Game.h"
#include "raylib.h" // Include raylib for timing functions and etc since only renderer is abstracted 

Game::Game()
    : m_renderer(1200, 720, "Space Shooter")
    , m_player({ 640.0f, 600.0f })
    , m_projectileSpeed(500.0f)
    , m_gameTimer(150.f)
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
    m_scoreSystem.LoadHighscore(m_world);
}

void Game::Run()
{
    while (!m_renderer.ShouldClose())
    {   
        m_profiler.BeginFrame();

        const float deltaTime = GetFrameTime();

        Update(deltaTime);
        Render();

        m_profiler.EndFrame();
    }
}



void Game::Update(float deltaTime)
{   

    if(IsKeyPressed(KEY_F3))
    {
        m_showProfiler = !m_showProfiler;
    }



    m_profiler.Begin("GameUpdate");

    //Game state check
    if (m_gameState == GameState::GameOver)
    {
        if (IsKeyPressed(KEY_R))
        {
            RestartGame();
        }
        m_profiler.End("GameUpdate");
        return;
    }

    //----------------- Timer ----------------
    m_gameTimer.Update(deltaTime);
    if (m_gameTimer.IsFinished())
    {
        EndGame();

        m_profiler.End("GameUpdate");
        return;
    }


    //---------------- Powerup System ----------------

    m_profiler.Begin("Powerup");

    m_powerupSystem.Update(
        m_world,
        m_player,
        m_powerupDatabse,
        deltaTime);

    m_profiler.End("Powerup");

    // ---------------- Player ----------------

    m_profiler.Begin("Player");

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

    m_profiler.End("Player");

    // ---------------- Enemy Spawning ----------------

    m_profiler.Begin("Spawn");

    m_spawnSystem.Update(
        m_world,
        m_enemyDatabase,
        deltaTime);

    m_profiler.End("Spawn");

    // ---------------- Enemy AI ----------------

    m_profiler.Begin("AI");

    m_aiSystem.Update(
        m_world,
        m_enemyDatabase,
        m_player.GetPosition(),
        deltaTime);

    m_profiler.End("AI");


    // ---------------- Weapon System ----------------

    m_profiler.Begin("Weapon");

    m_weaponSystem.Update(
        m_world,
        m_player,
        m_enemyDatabase,
        m_weaponDatabase,
        m_player.GetPosition(),
        deltaTime
    );

    m_profiler.End("Weapon");

    // ---------------- Movement ----------------

    m_profiler.Begin("Movement");

    m_movementSystem.Update(
        m_world,
        deltaTime);

    m_profiler.End("Movement");

    // ---------------- Projectiles ----------------

    m_profiler.Begin("Projectile");

    m_projectileSystem.Update(
        m_world,
        deltaTime);

    m_profiler.End("Projectile");

    // ---------------- Collision ----------------

    m_profiler.Begin("Collision");

    m_collisionSystem.Update(
        m_world,m_player, m_enemyDatabase);

    if(m_player.GetHealth() <= 0.0f)
    {
        EndGame();
    }

    m_profiler.End("Collision");

    //---------------- Score System ----------------

    m_profiler.Begin("Score");

    m_scoreSystem.Update(
        m_world,
        deltaTime);

    m_profiler.End("Score");

    // ---------------- Cleanup ----------------

    m_profiler.Begin("Cleanup");

    CleanupProjectiles(m_world);
    CleanupEnemies(m_world);

    m_profiler.End("Cleanup");

    m_profiler.End("GameUpdate");
}

void Game::Render()
{   
    m_profiler.Begin("Render");

    m_renderer.BeginFrame(); // Start drawing
    m_player.Render(); // Render the player
    m_renderer.Render(m_world); // Render the projectiles
    m_renderer.DrawHUD(m_player, m_world, m_gameTimer.GetRemainingTime()); // Render the HUD
    if(m_gameState == GameState::GameOver)
    {
        m_renderer.DrawGameOver(m_world); // Render the game over screen
    }

    if(m_showProfiler)
    {
        m_renderer.DrawProfiler(m_profiler); // Render the profiler
    }

    m_renderer.EndFrame(); // Finish drawing 

    m_profiler.End("Render");
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
    size_t i = 0;

    while (i < world.enemies.size())
    {
        if (world.enemyHealth[i].currentHealth > 0.0f)
        {
            ++i;
            continue;
        }

        // Capture information before removal.
        const Enemy enemy =
            world.enemies[i];

        const Vector2 deathPosition =
            world.enemyPositions[i].value;

        m_scoreSystem.RegisterEnemyDeath(
            world,
            enemy,
            deathPosition,
            m_enemyDatabase
        );

        const size_t last =
            world.enemies.size() - 1;

        if (i != last)
        {
            world.enemies[i] =
                world.enemies[last];

            world.enemyHealth[i] =
                world.enemyHealth[last];

            world.enemyPositions[i] =
                world.enemyPositions[last];

            world.enemyVelocities[i] =
                world.enemyVelocities[last];
        }

        world.enemies.pop_back();
        world.enemyHealth.pop_back();
        world.enemyPositions.pop_back();
        world.enemyVelocities.pop_back();
    }
}

void Game::RestartGame()
{
    // Reset the game state
    m_gameState = GameState::Playing;
    m_world.score = 0;

    m_world.scorePopup.clear();
    m_gameTimer.Reset();

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

    m_spawnSystem.Reset();
}

void Game::EndGame()
{
    m_gameState = GameState::GameOver;

    while (!m_world.scorePopup.empty())
    {
        m_world.score += m_world.scorePopup.back().score;
        m_world.scorePopup.pop_back();
    }

    if (m_world.score > m_world.highScore)
    {
        m_world.highScore = m_world.score;
        m_scoreSystem.SaveHighscore(m_world);
    }
}




