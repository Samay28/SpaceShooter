#pragma once

#include "Renderer.h"
#include "Player.h"
#include <vector>
#include "Projectile.h" //want able to fwd declare Projectile class because we need to store a vector of projectiles in the Game class, it needs to know the size of the Projectile class to allocate memory for the vector.

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
    void UpdateProjectiles(float deltaTime);    
    void RenderProjectile();
    void RemoveDeadProjectiles(); // Remove projectiles that are no longer alive

private:
    Renderer m_renderer;
    Player m_player;

    //--------------Projectile Management----------------
    std::vector<Projectile> m_projectiles; // Store projectiles in a vector
    float m_projectileSpeed = 400.0f; // Speed of the projectiles
};