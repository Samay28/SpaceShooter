#pragma once

#include "Renderer.h"
#include "Player.h"
#include <vector>

class Projectile; // Forward declaration of Projectile class

class Game
{
public:
    Game();

    void Run(); // Main game loop


private:
    void Update(float deltaTime);
    void Render();

private:
    Renderer m_renderer;
    Player m_player;

    //std::vector<Projectile> m_projectiles; // Store projectiles in a vector
};