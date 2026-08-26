#pragma once

#include "Renderer.h"
#include "Player.h"
#include <vector>
#include "GameWorld.h"
#include "MovementSystem.h"
#include "ProjectileSystem.h"

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

private:
    GameWorld m_world;

    MovementSystem m_movementSystem;
    ProjectileSystem m_projectileSystem;

    Renderer m_renderer;
    Player m_player;

    //--------------Projectile Management----------------
    float m_projectileSpeed = 400.0f; // Speed of the projectiles
};