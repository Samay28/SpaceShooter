#pragma once
#include "raylib.h"

class Projectile; // Forward declaration of Projectile class
class Player
{

public:
    explicit Player(Vector3 startPos);

    void Update(float deltaTime, float speedMultiplier);
    void Render() const;



    bool IsShootPressed() const;

    void Fire();
    Vector2 GetProjectileSpawnPos() const;

    Vector2 GetPosition() const { return m_position; }

    void TakeDamage(float damage);
    float GetHealth() const { return m_health; }
    void Heal(float amount);
    float GetMaxHealth() const { return m_maxHealth; }

    void SetShootInterval(float interval) { m_shootInterval = interval; }
    float GetShootInterval() const { return m_shootInterval; }

    //move speed getter and setter
    float GetMoveSpeed() const { return m_moveSpeed; }
    void SetMoveSpeed(float speed) { m_moveSpeed = speed; }

private:
    void Move(float deltaTime, float speedMiltiplier);
    void UpdateCooldown(float deltaTime);

private:

    Vector2 m_position;
    Vector2 m_velocity;

    float m_moveSpeed;

    float m_shootCooldown;
    float m_shootInterval;

    float m_health;
    float m_maxHealth;
};

