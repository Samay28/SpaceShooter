#pragma once
#include "raylib.h"

class Projectile; // Forward declaration of Projectile class
class Player
{

public:
    explicit Player(Vector3 startPos);

    void Update(float deltaTime);
    void Render() const;


    bool WantsToShoot() const;
    void Fire();
    Vector2 GetProjectileSpawnPos() const;
private:
    void Move(float deltaTime);
    void UpdateCooldown(float deltaTime);

private:

    Vector2 m_position;
    Vector2 m_velocity;

    float m_moveSpeed;

    float m_shootCooldown;
    float m_shootInterval;
};

