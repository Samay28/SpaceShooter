#include "Projectile.h"

Projectile::Projectile(Vector2 pos, Vector2 vel)
    : m_position{pos}
    , m_velocity{vel}
    , m_alive{true}
{
}

void Projectile::Update(float deltaTime)
{
    m_position.x += m_velocity.x * deltaTime;
    m_position.y += m_velocity.y * deltaTime;

    // Check if the projectile is out of screen bounds
    if (m_position.y < -20.0f ||
        m_position.y > GetScreenHeight() + 20.0f ||
        m_position.x < -20.0f ||
        m_position.x > GetScreenWidth() + 20.0f)
    {
        m_alive = false;
    }
}

void Projectile::Render() const
{
    DrawCircleV(m_position, 5.0f, RED);
}

bool Projectile::isAlive() const
{
    return m_alive;
}