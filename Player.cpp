#include "Player.h"
#include "raymath.h"

Player::Player(Vector3 startPos)
    : m_position{startPos.x, startPos.y}
    , m_velocity{0.0f, 0.0f}
    , m_moveSpeed{200.0f}
    , m_shootCooldown{0.0f}
    , m_shootInterval{0.5f} // 0.5 seconds between shots
    , m_health{ 100.0f }
    , m_maxHealth{ 100.0f }
{
}

void Player::Update(float deltaTime, float speedMultiplier)
{
    Move(deltaTime, speedMultiplier);
    UpdateCooldown(deltaTime);
}

void Player::TakeDamage(float damage)
{
    m_health -= damage;
    if (m_health < 0.0f)
    {
        m_health = 0.0f; // Ensure health doesn't go below zero
    }
}

void Player::Heal(float amount)
{
    m_health += amount;
    if (m_health > m_maxHealth)
    {
        m_health = m_maxHealth; // Ensure health doesn't exceed max health
    }
}


void Player::Reset()
{
    m_position = {
        640.0f,
        600.0f
    };

    m_velocity = {
        0.0f,
        0.0f
    };

    m_health =
        m_maxHealth;

    m_shootCooldown =
        0.0f;
}

void Player::Move(float deltaTime, float speedMultiplier)
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

        const float effectiveSpeed = m_moveSpeed * speedMultiplier;

        m_position.x += m_velocity.x * effectiveSpeed * deltaTime;
        m_position.y += m_velocity.y * effectiveSpeed * deltaTime;
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

bool Player::IsShootPressed() const
{
    return IsKeyDown(KEY_SPACE) && m_shootCooldown <=0.f;
}

void Player::Fire()
{
    m_shootCooldown = m_shootInterval; // Reset the cooldown timer after shooting
}

void Player::UpdateCooldown(float deltaTime)
{
    if (m_shootCooldown > 0.0f)
    {
        m_shootCooldown -= deltaTime;

        if (m_shootCooldown < 0.0f)
        {
            m_shootCooldown = 0.0f;
        }
    }
}


Vector2 Player::GetProjectileSpawnPos() const
{
    return { m_position.x, m_position.y - 18.0f }; // Spawn the projectile slightly above the player
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