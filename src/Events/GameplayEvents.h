#pragma once
#include "Event.h"
#include "raylib.h"

#include "Components/Enemy.h"
#include "Components/Powerup.h"

#include <cstddef>
#include <Data/ProjectileData.h>

// ============================================================
// EnemySpawnedEvent
// ============================================================
//
// Published when a new enemy is created.
//
// This lets other systems know:

struct EnemySpawnedEvent : public Event
{
    EnemyType type;
    Vector2 position;
};

// ============================================================
// EnemyKilledEvent
// ============================================================

struct EnemyKilledEvent : public Event
{
    EnemyType type;
    Vector2 position;
    int score;
};


// ============================================================
// ProjectileFiredEvent
// ============================================================

struct ProjectileFiredEvent : public Event
{
    ProjectileOwner owner;
    Vector2 position;
    int count;
};

// ============================================================
// PlayerDamagedEvent
// ============================================================

struct PlayerDamagedEvent : public Event
{
    float damage;
    float remainingHealth;
};

// ============================================================
// PowerupCollectedEvent
// ============================================================

struct PowerupCollectedEvent : public Event
{
    PowerupType type;
    Vector2 position;
};

// ============================================================
// GameOverEvent
// ============================================================
struct GameOverEvent : public Event
{
    int finalScore;
    int highScore;
};