# SpaceShooter

A small top-down 2D space shooter built in **C++ and Raylib**, designed as a practical exploration of **Data-Oriented Design (DOD)**, data-driven gameplay, and system-based architecture.

The goal of the project is to keep gameplay code simple while demonstrating how runtime game data can be organized and processed efficiently.

---

## Controls

| Input             | Action                  |
| ----------------- | ----------------------- |
| **W / A / S / D** | Move                    |
| **SPACE**         | Shoot                   |
| **R**             | Restart after Game Over |
| **ESC**           | Quit                    |

---

## Gameplay

Survive for **150 seconds** while fighting incoming enemies.

Enemies spawn from the top of the screen, move into combat positions, and attack the player. Different enemy types have different stats and weapons.

Destroy enemies to earn score and collect powerups to temporarily improve the player's abilities.

The game ends when:

* Player health reaches **0**
* The **150-second timer** reaches **0**

### Scoring

When an enemy is destroyed, a floating `+Score` popup appears at its death position.

The popup remains visible for **1 second**, after which its value is added to the player's total score.

The highest score is saved locally and displayed on the Game Over screen.

---

## Enemy Types

### Basic

* Standard enemy 
* Normal movement speed
* Normal health
* Fires a single projectile

### Duo

* Higher health than the Basic enemy
* Fires **two projectiles simultaneously**

### Fast

* Lower health
* Higher movement speed
* Fires a **fast projectile**

### Tank

* Very high health
* Slow movement
* Built to take significantly more damage

Enemy properties such as health, movement speed, weapon, attack duration, radius, and score value are loaded from external data files.

---

## Powerups

### Double Shot (Pink Color)

Temporarily causes the player to fire **two projectiles**.

### Triple Shot (Purple Color)

Temporarily causes the player to fire **three projectiles**.

### Rapid Fire (Yellow Color)

Temporarily increases the player's firing rate.

### Speed Boost (Orange Color)

Temporarily increases player movement speed.

### Shield (Skyblue Color)

Temporarily protects the player from damage.

### Heal (Green Color)

Instantly restores a portion of player health.

Powerup properties such as duration and effect value are defined externally in `assets/powerups.txt`.

---

# Technical Implementation

## Technology

* **C++**
* **Raylib**
* **CMake / Visual Studio**
* Standard C++ containers and libraries

No game engine such as Unity or Unreal Engine is used.

---

## Architecture

The project is organized around a **data + systems** approach.

```text
Game
 │
 ├── GameWorld
 │      ├── Runtime Data
 │      ├── Positions
 │      ├── Velocities
 │      ├── Enemies
 │      ├── Projectiles
 │      └── Powerups
 │
 ├── SpawnSystem
 ├── AISystem
 ├── MovementSystem
 ├── WeaponSystem
 ├── ProjectileSystem
 ├── CollisionSystem
 ├── PowerupSystem
 ├── ScoreSystem
 │
 └── Renderer
```

### GameWorld

`GameWorld` owns the runtime gameplay data.

For example:

```cpp
std::vector<Position> projectilePositions;
std::vector<Velocity> projectileVelocities;
std::vector<Projectile> projectiles;
```

The same index represents the same projectile across the arrays.

This keeps frequently processed data grouped by purpose rather than storing everything inside large objects.

---

## Data-Oriented Design

The project uses a simplified **Structure of Arrays (SoA)** style for runtime entities.

Instead of:

```cpp
struct Projectile
{
    Position position;
    Velocity velocity;
    float damage;
    float lifetime;
};
```

runtime data is separated:

```cpp
std::vector<Position> projectilePositions;
std::vector<Velocity> projectileVelocities;
std::vector<Projectile> projectiles;
```

Systems can then operate on the data they actually need.

For example, movement only needs:

```text
Position + Velocity
```

while projectile lifetime processing only needs:

```text
Projectile lifetime
```

The intention is to make the data layout more cache-friendly and reduce unnecessary data access during system updates.

---

## Systems

Gameplay behavior is separated into independent systems.

### SpawnSystem

Responsible for creating:

* Enemies
* Projectiles
* Powerups

### AISystem

Controls enemy behavior such as:

* Moving toward target positions
* Switching between movement and attacking
* Selecting new combat positions
* Managing attack timers

### MovementSystem

Updates entity positions using velocity data.

```text
Position += Velocity × DeltaTime
```

### WeaponSystem

Responsible for weapon behavior and projectile creation.

Weapons are data-driven, allowing different enemies to use different weapon configurations.

### ProjectileSystem

Handles projectile-specific runtime updates such as lifetime.

Projectile movement itself is handled by `MovementSystem`.

### CollisionSystem

Handles interactions between:

* Player and enemy projectiles
* Player and enemies
* Projectiles and enemies
* Player and powerups

### PowerupSystem

Handles:

* Powerup pickup
* Activating effects
* Active powerup timers
* Expiring temporary effects
* Instant effects such as healing

### ScoreSystem

Handles:

* Enemy death score registration
* Floating score popups
* Delayed score application
* High score loading/saving

---

# Data-Driven Gameplay

Gameplay definitions are separated from runtime state.

Enemy configuration is stored in:

```text
assets/enemies.txt
```

Example:

```text
[Basic]
Health=50
Speed=80
Radius=15
AttackDurationMin=1.5
AttackDurationMax=3.0
Weapon=Single
Score=50
```

Weapon configuration is stored in:

```text
assets/weapons.txt
```

Powerup configuration is stored in:

```text
assets/powerups.txt
```

This means gameplay values can be changed without modifying the systems themselves.

For example, changing:

```text
Health=50
```

to:

```text
Health=100
```

changes the enemy's health without requiring a change to the enemy AI or spawn code.

---

## Runtime Data vs Definitions

The project separates **static definitions** from **runtime state**.

### Definitions

Describe what something is:

```cpp
struct EnemyDefinition
{
    float maxHealth;
    float moveSpeed;
    float radius;
    float attackDurationMin;
    float attackDurationMax;
    WeaponType weaponType;
    int score;
};
```

### Runtime Data

Describes what is currently happening:

```cpp
struct Enemy
{
    EnemyType type;
    EnemyState state;
    Vector2 targetPosition;
    float stateTimer;
    float weaponCooldown;
};
```

This allows many runtime enemies to reference the same static definition instead of duplicating configuration data for every instance.

---

## Rendering

Rendering is separated from gameplay systems.

`Renderer` reads data from `GameWorld` and draws:

* Player
* Enemies
* Projectiles
* Powerups
* Health bar
* Score
* Timer
* Active powerups
* Score popups
* Game Over screen

The renderer does not decide gameplay behavior; it only visualizes the current game state.

---

## Persistence

The high score is stored outside the asset definitions:

```text
save/highscore.txt
```

At startup:

```text
Load previous high score
```

At Game Over:

```text
Current Score > High Score
        ↓
Update High Score
        ↓
Save to file
```

The high score survives game restarts and new play sessions.

---

## Project Structure

```text
SpaceShooter/
│
├── assets/
│   ├── enemies.txt
│   ├── weapons.txt
│   └── powerups.txt
│
├── save/
│   └── highscore.txt
│
└── src/
    ├── Core/
    ├── Components/
    ├── Data/
    ├── Systems/
    ├── Rendering/
    ├── Player.h
    ├── Player.cpp
    ├── Game.h
    ├── Game.cpp
    └── main.cpp
```

---

## Project Goals

This project is primarily an architecture and programming exercise.

The main goals are to explore:

* Data-Oriented Design
* Structure of Arrays data layouts
* Data-driven gameplay
* Separation of runtime data and static definitions
* System-based gameplay architecture
* Efficient iteration over large numbers of entities
* Profiling and performance analysis

The project intentionally avoids introducing a full ECS framework unless profiling demonstrates that it is necessary.

