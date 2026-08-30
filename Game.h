#pragma once

#include "Renderer.h"
#include "Player.h"
#include <vector>
#include "GameWorld.h"
#include "MovementSystem.h"
#include "SpawnSystem.h"
#include "AISystem.h"
#include "CollisionSystem.h"
#include "ProjectileSystem.h"

#include "EnemyDatabase.h"
#include "WeaponDatabase.h"
#include "WeaponSystem.h"

class Game
{
public:
    Game();

    void Run(); // Main game loop


private:
    void Update(float deltaTime);
    void Render();

    //--------------Projectile Management----------------
    void HandlePlayerShooting();
    void CleanupProjectiles(GameWorld& world); // Remove dead projectiles from the vector


    //--------------Enemy Management----------------
    void CleanupEnemies(GameWorld& world);

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


    //--------------Projectile Management----------------
    float m_projectileSpeed = 400.0f; // Speed of the projectiles
};