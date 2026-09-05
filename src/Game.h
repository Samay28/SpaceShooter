#pragma once

#include "Renderer.h"
#include "Player.h"
#include <vector>
#include "Core/GameWorld.h"

#include "Systems/MovementSystem.h"
#include "Systems/SpawnSystem.h"
#include "Systems/AISystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/ProjectileSystem.h"
#include "Systems/WeaponSystem.h"
#include "Systems/PowerupSystem.h"

#include "Data/EnemyDatabase.h"
#include "Data/WeaponDatabase.h"

#include "Core/GameTimer.h"
#include "Systems/ScoreSystem.h"


#include "Debug/Profiler.h"

enum class GameState
{
    Playing,
    GameOver
};

class Game
{
public:
    Game();

    void Run(); // Main game loop


private:
    void Update(float deltaTime);
    void Render();

    //--------------Projectile Management----------------
    /*void HandlePlayerShooting();*/
    void CleanupProjectiles(GameWorld& world); // Remove dead projectiles from the vector


    //--------------Enemy Management----------------
    void CleanupEnemies(GameWorld& world);

    void RestartGame();
    void EndGame();

private:
    GameWorld m_world;

    MovementSystem m_movementSystem;
    ProjectileSystem m_projectileSystem;

    Renderer m_renderer;
    Player m_player;

    SpawnSystem m_spawnSystem;
    AISystem m_aiSystem;
    CollisionSystem m_collisionSystem;
    WeaponSystem m_weaponSystem;

    EnemyDatabase m_enemyDatabase;
    WeaponDatabase m_weaponDatabase;

    PowerupSystem m_powerupSystem;
    PowerupDatabase m_powerupDatabse;

    Profiler m_profiler;
    bool m_showProfiler = false;

    //--------------Projectile Management----------------
    float m_projectileSpeed = 400.0f; // Speed of the projectiles


    GameState m_gameState = GameState::Playing;


    GameTimer m_gameTimer{ 150.f };
    ScoreSystem m_scoreSystem;
};