#include "Player.h"
#include "raymath.h"

Player::Player(Vector3 startPos)
    : m_position{startPos.x, startPos.y}
    , m_velocity{0.0f, 0.0f}
    , m_moveSpeed{200.0f}
    , m_shootCooldown{0.0f}
    , m_shootInterval{0.5f} // 0.5 seconds between shots
{
}

void Player::Update(float deltaTime)
{
    Move(deltaTime);
    Shoot();

    if(m_shootCooldown > 0.0f)
    {
        m_shootCooldown -= deltaTime;
    }
}

void Player::Move(float deltaTime)
{
    m_velocity = { 0.f,0.f };
    if(IsKeyDown(KEY_W))
    {
        m_velocity.y -= 1.0f;
    }

    if(IsKeyDown(KEY_S))
    {
        m_velocity.y += 1.0f;
    }

    if(IsKeyDown(KEY_A))
    {
        m_velocity.x -= 1.0f;
    }

    if(IsKeyDown(KEY_D))
    {
        m_velocity.x += 1.0f;
    }

    if (Vector2Length(m_velocity) > 0.0f)
    {   
        // Normalize the velocity vector to ensure consistent movement speed in all directions
        m_velocity = Vector2Normalize(m_velocity);

        m_position.x += m_velocity.x * m_moveSpeed * deltaTime;
        m_position.y += m_velocity.y * m_moveSpeed * deltaTime;
    }

    // Keep player inside screen.
    m_position.x = Clamp(
        m_position.x,
        20.0f,
        static_cast<float>(GetScreenWidth() - 20) // 20 pixels from the right edge
    );

    m_position.y = Clamp(
        m_position.y,
        20.0f,
        static_cast<float>(GetScreenHeight() - 20) // 20 pixels from the bottom edge
    );
}

void Player::Shoot()
{
    if (IsKeyDown(KEY_SPACE) && m_shootCooldown <= 0.f)
    {
        TraceLog(LOG_INFO, "Player shoots!");
        m_shootCooldown = m_shootInterval; // Reset the cooldown
    }
}

void Player::Render() const
{
    DrawTriangle(
        {
            m_position.x,
            m_position.y - 15.0f
        },
        {
            m_position.x - 12.0f,
            m_position.y + 12.0f
        },
        {
            m_position.x + 12.0f,
            m_position.y + 12.0f
        },
        SKYBLUE
    );
}