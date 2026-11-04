#include "Game.h"
#include <iostream>
#include "raylib.h" // Include raylib for timing functions and etc since only renderer is abstracted 




Game::Game()
    : m_renderer(1200, 720, "Space Shooter")
    , m_player({ 640.0f, 600.0f })
    , m_projectileSpeed(500.0f)
    , m_gameTimer(150.f)
    , m_gameBranchController()
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
    SetTargetFPS(144);
    startGameSound = LoadSound("assets/sounds/start_game.wav");
    bgMusic = LoadMusicStream("assets/sounds/bg_music.wav");
    gameOverSound = LoadSound("assets/sounds/game_over.wav");
    PlaySound(startGameSound);
    PlayMusicStream(bgMusic);


    m_scoreSystem.SetGameBranchController(
        &m_gameBranchController
    );
    
}

void Game::Run()
{
    while (!m_renderer.ShouldClose())
    {   
        m_profiler.BeginFrame();
        //--------------------------Spiral of Death-----------------------------
        // The game tries to simulate time that has already passed by running multiple physics 
        // or update steps


        // moved out of the updatesim, as it will not open when target fps is 30, simulation step will be called twice
        // making toggle the profiler on and off in one frame, so it will never be visible
        if (IsKeyPressed(KEY_F3))
        {
            m_showProfiler = !m_showProfiler;
        }

        if(IsKeyPressed(KEY_P))
        {
            PauseGame();
        }
        if(IsKeyPressed(KEY_R) && m_gameState == GameState::GameOver)
        {
            RestartGame();
        }

        // Limit frame time to avoid spiral of death
        const float rawFrameTime = GetFrameTime();

        if (rawFrameTime > 0.25f)
        {
            m_accumulator = 0.0f;
        }

        const float frameTime = std::min(rawFrameTime, 0.25f);

        m_accumulator += frameTime; //accumulate the time since the last update
        m_simulationStepsThisFrame = 0; // Count the number of simulation steps taken in this frame
        // Fixed time step loop to ensure consistent physics and game logic updates

        if (m_gameState == GameState::Playing)
        {
            while (m_accumulator >= FixedDeltaTime && m_simulationStepsThisFrame < MaxSimSteps)
            {   
                UpdateSimulation(FixedDeltaTime);
                m_accumulator -= FixedDeltaTime; //decrease the accumulator by the fixed time step
                ++m_simulationStepsThisFrame; // Increment the simulation step counter

                
            }
        }
        else if (m_gameState == GameState::CutScene)
        {
            UpdateCutScene(FixedDeltaTime);
            m_accumulator = 0.0f;
            m_simulationStepsThisFrame = 0; // Reset the simulation step counter when the game is paused or over
        }
        else
        {
            //prevent accumulated real time from being simulated
            //when the game resumes
            m_accumulator = 0.0f;
            m_simulationStepsThisFrame = 0; // Reset the simulation step counter when the game is paused or over
        }
        Render();
       

        m_profiler.EndFrame();
    }
}



void Game::UpdateSimulation(float fixedDeltaTime)
{   

    m_profiler.Begin("Simulation");

    //Game state check
    if (m_gameState == GameState::GameOver)
    {
        m_profiler.End("Simulation");
        return;
    }
    UpdateMusicStream(bgMusic);
    //----------------- Timer ----------------
    m_gameTimer.Update(fixedDeltaTime);


    if (m_gameTimer.CheckForCutsceneTime(140.0f))
    {
        BeginCutscene();
        m_profiler.End("Simulation");
        return;
    }


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
        fixedDeltaTime);

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

    m_player.Update(fixedDeltaTime, speedMultiplier);
    
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
        fixedDeltaTime);

    m_profiler.End("Spawn");

    // ---------------- Enemy AI ----------------

    m_profiler.Begin("AI");

    m_aiSystem.Update(
        m_world,
        m_enemyDatabase,
        m_player.GetPosition(),
        FixedDeltaTime);

    m_profiler.End("AI");


    // ---------------- Weapon System ----------------

    m_profiler.Begin("Weapon");

    m_weaponSystem.Update(
        m_world,
        m_player,
        m_enemyDatabase,
        m_weaponDatabase,
        m_player.GetPosition(),
        fixedDeltaTime
    );

    m_profiler.End("Weapon");

    // ---------------- Movement ----------------

    m_profiler.Begin("Movement");

    m_movementSystem.Update(
        m_world,
        fixedDeltaTime);

    m_profiler.End("Movement");

    // ---------------- Projectiles ----------------

    m_profiler.Begin("Projectile");

    m_projectileSystem.Update(
        m_world,
        fixedDeltaTime);

    m_profiler.End("Projectile");

    //---------------- Spatial Grid ----------------

    //build the broad phase structure using the latest positions of the enemies
    m_spatialGrid.BuildEnemyGrid(m_world.enemyPositions);

    // ---------------- Collision ----------------

    m_profiler.Begin("Collision");

    m_collisionSystem.Update(
        m_world,m_player, m_enemyDatabase, m_spatialGrid);

    if(m_player.GetHealth() <= 0.0f)
    {
        EndGame();
    }

    m_profiler.End("Collision");

    //---------------- Score System ----------------

    m_profiler.Begin("Score");

    m_scoreSystem.Update(
        m_world,
        fixedDeltaTime);

    m_profiler.End("Score");

    // ---------------- Cleanup ----------------

    m_profiler.Begin("Cleanup");

    CleanupProjectiles(m_world);
    CleanupEnemies(m_world);

    m_profiler.End("Cleanup");

    m_profiler.End("Simulation");
}

void Game::UpdateCutScene(float fixedDeltaTime)
{
    // Only update the final dialogue countdown here.
    if (m_dialogueDisplayMode == DialogueDisplayMode::TimedMessage)
    {
        m_cutSceneTimer -= fixedDeltaTime;

        if (m_cutSceneTimer <= 0.0f)
        {
            m_cutSceneTimer = 0.0f;

            m_cutSceneImposter.visible = false;

            // End the cutscene permanently.
            m_gameState = GameState::Playing;

            m_dialogueDisplayMode = DialogueDisplayMode::Choices;
        }
    }

    if (m_dialogueDisplayMode == DialogueDisplayMode::Choices)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 mousePosition = GetMousePosition();

            if (CheckCollisionPointRec(
                mousePosition,
                Renderer::GetChoiceButtonRect(0)))
            {
                // Select Accept.
                m_gameBranchController.SelectChoice(0);

                m_dialogueDisplayMode = DialogueDisplayMode::Continue;
            }
            else if (CheckCollisionPointRec(
                mousePosition,
                Renderer::GetChoiceButtonRect(1)))
            {
                // Select Reject.
                m_gameBranchController.SelectChoice(1);
                m_spawnSystem.SpawnImposter(m_world, m_enemyDatabase, false, m_gameBranchController.findImposter());
                m_dialogueDisplayMode = DialogueDisplayMode::Continue;
            }
        }
    }
    if (m_dialogueDisplayMode == DialogueDisplayMode::Continue)
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            // Assumes nodes 1 and 2 each have one outgoing
            // Continue choice leading to node 3 or node 4.
            m_gameBranchController.SelectChoice(0);

            m_dialogueDisplayMode = DialogueDisplayMode::TimedMessage;
            m_cutSceneTimer = 6.0f;
        }
    }
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
        m_renderer.DrawProfiler(m_profiler, FixedDeltaTime, m_simulationStepsThisFrame); // Render the profiler
    }

    if (m_gameState == GameState::CutScene &&
        m_cutSceneImposter.visible)
    {
        m_renderer.DrawCutsceneImposter(
            m_cutSceneImposter.type,
            m_cutSceneImposter.position,
            m_player.GetPosition()
        );
        // Get the current dialogue node.
        const DialogueNode* currentNode =
            m_gameBranchController.GetCurrentNode();

        // Draw the dialogue UI on top of the world.
        if (currentNode != nullptr)
        {
            m_renderer.DrawCutsceneUI(
                *currentNode,
                m_dialogueDisplayMode,
                m_cutSceneTimer
            );
        }
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
    m_world.ClearAll();
    m_spatialGrid.Clear(); // Clear the spatial grid

    m_world.playerPowerups.clear();

    m_spawnSystem.Reset();

    PlaySound(startGameSound);
    PlayMusicStream(bgMusic);

    //reset the game branch controller
    m_gameBranchController.BuildGraph();
    m_gameBranchController.resetEnemyKillCount();
}

void Game::EndGame()
{
    m_gameState = GameState::GameOver;
    PlaySound(gameOverSound);
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

void Game::PauseGame()
{
    //for now just pause the game, but we can add a pause menu later

    // Toggle the game state between Playing and Paused
    if (m_gameState == GameState::Playing)
    {
        m_gameState = GameState::Paused;
    }
    else if (m_gameState == GameState::Paused)
    {
        m_gameState = GameState::Playing;
    }


}

void Game::BeginCutscene()
{
    m_world.ClearAll();
    m_spatialGrid.Clear();

    m_cutSceneImposter.type = m_gameBranchController.findImposter();

    m_cutSceneImposter.position = {
      GetScreenWidth() * 0.5f,
      GetScreenHeight() * 0.5f
    };
    m_cutSceneImposter.visible = true;

    m_dialogueDisplayMode = DialogueDisplayMode::Choices; //always start here
    m_cutSceneTimer = 0.f; //no countdown till we reach that point

    m_gameState = GameState::CutScene;
  
}




