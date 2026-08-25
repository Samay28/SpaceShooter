#pragma once
#include "raylib.h"

class Projectile
{
public:
    Projectile(Vector2 pos, Vector2 vel);

    void Update(float deltaTime);
    void Render() const;

    bool isAlive() const;

private:

    Vector2 m_position;
    Vector2 m_velocity;

    bool m_alive;

};

