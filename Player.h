#pragma once
#include "raylib.h"

class Player
{

public:
    explicit Player(Vector3 startPos);

    void Update(float deltaTime);
    void Render() const;


private:
    void Move(float deltaTime);
    void Shoot();

private:

    Vector2 m_position;
    Vector2 m_velocity;

    float m_moveSpeed;

    float m_shootCooldown;
    float m_shootInterval;
};

