#include "Renderer.h"
#include "Core/GameWorld.h"
#include "Player.h"
#include "raylib.h"

Renderer::Renderer(
    int screenWidth,
    int screenHeight,
    const char* title)
    : m_screenWidth(screenWidth)
    , m_screenHeight(screenHeight)
{
    InitWindow(
        m_screenWidth,
        m_screenHeight,
        title
    );

    SetTargetFPS(60);
}

Renderer::~Renderer()
{
    CloseWindow();
}

void Renderer::BeginFrame()
{
    BeginDrawing();

    ClearBackground(BLACK);
}

void Renderer::EndFrame()
{
    EndDrawing();
}

bool Renderer::ShouldClose() const
{
    return WindowShouldClose();
}

void Renderer::Render(const GameWorld& world)
{   
    // Render player projectiles
    for (size_t i = 0; i < world.projectiles.size(); ++i)
    {   
        const Projectile& projectile = world.projectiles[i];
        if (projectile.owner == ProjectileOwner::Player)
        {
            const Vector2 pos = world.projectilePositions[i].value;
            DrawCircleV(pos, 4.0f, YELLOW);
        }
    }

    //Render enemy projectiles
    for(size_t i = 0; i < world.projectiles.size(); ++i)
    {
        const Projectile& projectile = world.projectiles[i];
        if (projectile.owner == ProjectileOwner::Enemy)
        {
            const Vector2 pos = world.projectilePositions[i].value;
            DrawCircleV(pos, 4.0f, RED);
        }
    }

    // Render enemies
    for(size_t i=0; i<world.enemies.size(); ++i)
    {
        DrawCircleV(
            world.enemyPositions[i].value,
            15.f,
            RED);
    }

    //Render Powerups
    for (size_t i = 0; i < world.powerups.size(); ++i)
    {
        const Vector2 position = world.powerupPositions[i].value;
        Color color = WHITE;

        switch (world.powerups[i].type)
        {
        case PowerupType::DoubleShot:
            color = BLUE;
            break;

        case PowerupType::TripleShot:
            color = PURPLE;
            break;

        case PowerupType::RapidFire:
            color = YELLOW;
            break;

        case PowerupType::SpeedBoost:
            color = ORANGE;
            break;

        case PowerupType::Shield:
            color = SKYBLUE;
            break;

        case PowerupType::Heal:
            color = GREEN;
            break;
        }

        DrawCircleV(
            position,
            10.0f,
            color
        );
       
        DrawScorePopups(world);

    }
}

void Renderer::DrawPowerupUI(const GameWorld& world)
{
    int y = 90;

    for (const ActivePowerup& powerup :
        world.playerPowerups)
    {
        const char* name = "UNKNOWN";

        switch (powerup.type)
        {
        case PowerupType::DoubleShot:
            name = "DOUBLE SHOT";
            break;

        case PowerupType::TripleShot:
            name = "TRIPLE SHOT";
            break;

        case PowerupType::RapidFire:
            name = "RAPID FIRE";
            break;

        case PowerupType::SpeedBoost:
            name = "SPEED BOOST";
            break;

        case PowerupType::Shield:
            name = "SHIELD";
            break;

        default:
            break;
        }

        DrawText(
            name,
            20,
            y,
            20,
            WHITE
        );

        DrawText(
            TextFormat(
                "%.1f",
                powerup.remainingTime
            ),
            180,
            y,
            20,
            WHITE
        );

        y += 25;
    }
}

void Renderer::DrawHealthBar(
    const Player& player)
{
    constexpr int x = 20;
    constexpr int y = 20;

    constexpr int width = 250;
    constexpr int height = 24;

    const float healthPercent =
        player.GetHealth() /
        player.GetMaxHealth();

    DrawRectangle(
        x,
        y,
        width,
        height,
        DARKGRAY
    );

    DrawRectangle(
        x,
        y,
        static_cast<int>(
            width * healthPercent
            ),
        height,
        RED
    );

    DrawRectangleLines(
        x,
        y,
        width,
        height,
        WHITE
    );

    DrawText(
        TextFormat(
            "HP %.0f / %.0f",
            player.GetHealth(),
            player.GetMaxHealth()
        ),
        x + 8,
        y + 3,
        16,
        WHITE
    );
}

void Renderer::DrawScorePopups(const GameWorld& world)
{
    for (const ScorePopup& popup : world.scorePopup)
    {
        DrawText(
            TextFormat("+%d", popup.score),
            static_cast<int>(popup.position.x),
            static_cast<int>(popup.position.y),
            20,
            YELLOW
        );
    }
}

void Renderer::DrawHUD(const Player& player, const GameWorld& world, float remainingTimer)
{
    DrawHealthBar(player);
    DrawText(TextFormat("SCORE : %d", world.score), 20, 55, 20, WHITE);
    DrawPowerupUI(world);
    DrawText(TextFormat("TIME: %03d", static_cast<int>(remainingTimer)), m_screenWidth - 170, 20, 28, WHITE);
}
 
void Renderer::DrawGameOver(const GameWorld& world)
{   
    //Dark overlay
    DrawRectangle(0, 0, m_screenWidth, m_screenHeight, Fade(BLACK, 0.75f));

    const int centerX = m_screenWidth / 2;
    const int centerY = m_screenHeight / 2;

    //Game Over

    const char* title = "GAME OVER";
    const int titleWidth = MeasureText(title, 50);

    DrawText(title, centerX - titleWidth / 2, centerY - 130, 50, RED);

    //Score
    const char* scoreText = TextFormat("SCORE: %d", world.score);
    const int scoreWidth = MeasureText(scoreText, 28);

    DrawText(scoreText, centerX- scoreWidth/2,centerY-40, 28, WHITE);

    //Highscore

    const char* highScoreText = TextFormat("HIGH SCORE: %d", world.highScore);
    const int highscoreWidth = MeasureText(highScoreText, 28);

    DrawText(highScoreText, centerX-highscoreWidth/2, centerY+10, 28, GOLD);

    const char* restartText = "Press R to Restart";
    const int restartTextWidth = MeasureText(restartText, 24); // Get the width of the text
    DrawText(restartText, centerX - restartTextWidth / 2, centerY + 80, 24, WHITE);
}
