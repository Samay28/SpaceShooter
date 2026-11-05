#include "Renderer.h"
#include "Core/GameWorld.h"
#include "raymath.h"
#include "Player.h"
#include "Core/GameBranchController.h"
#include "Debug/Profiler.h"

#include <sstream>

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
    m_basicEnemyTexture =
        LoadTexture("assets/enemies/basic.png");

    m_duoEnemyTexture =
        LoadTexture("assets/enemies/duo.png");

    m_fastEnemyTexture =
        LoadTexture("assets/enemies/fast.png");

    m_tankEnemyTexture =
        LoadTexture("assets/enemies/tank.png");

    m_doubleShotTexture =
        LoadTexture("assets/powerups/doubleshot.png");

    m_tripleShotTexture = 
        LoadTexture("assets/powerups/tripleshot.png");

    m_rapidFireTexture = 
        LoadTexture("assets/powerups/rapidfire.png");

    m_speedBoostTexture =
        LoadTexture("assets/powerups/speedboost.png");

    m_shieldTexture =
        LoadTexture("assets/powerups/shield.png");

    m_healthPackTexture =
        LoadTexture("assets/powerups/heal.png");

    m_backgroundTexture =
        LoadTexture("assets/backgrounds/background.png");

}

Renderer::~Renderer()
{
    CloseWindow();
    UnloadTexture(m_basicEnemyTexture);
    UnloadTexture(m_duoEnemyTexture);
    UnloadTexture(m_fastEnemyTexture);
    UnloadTexture(m_tankEnemyTexture);
    UnloadTexture(m_doubleShotTexture);
    UnloadTexture(m_tripleShotTexture);
    UnloadTexture(m_rapidFireTexture);
    UnloadTexture(m_speedBoostTexture);
    UnloadTexture(m_shieldTexture);
    UnloadTexture(m_healthPackTexture);
    UnloadTexture(m_backgroundTexture);
}

void Renderer::BeginFrame()
{
    BeginDrawing();

    ClearBackground(BLACK);
    DrawBackground();
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

    //render ally projectiles
    for(size_t i = 0; i < world.projectiles.size(); ++i)
    {
        const Projectile& projectile = world.projectiles[i];
        if (projectile.owner == ProjectileOwner::Ally)
        {
            const Vector2 pos = world.projectilePositions[i].value;
            DrawCircleV(pos, 4.0f, GREEN);
        }
    }

    // Render enemies
    for (size_t i = 0; i < world.enemies.size(); ++i)
    {
        const Enemy& enemy =
            world.enemies[i];

        const Vector2 position =
            world.enemyPositions[i].value;

        Texture2D texture{};

        switch (enemy.type)
        {
        case EnemyType::Basic:
            texture = m_basicEnemyTexture;
            break;

        case EnemyType::Duo:
            texture = m_duoEnemyTexture;
            break;

        case EnemyType::Fast:
            texture = m_fastEnemyTexture;
            break;

        case EnemyType::Tank:
            texture = m_tankEnemyTexture;
            break;
        }

        DrawTexturePro(
            texture,

            // Source rectangle: entire texture
            {
                0.0f,
                0.0f,
                static_cast<float>(texture.width),
                static_cast<float>(texture.height)
            },

            // Destination rectangle:
            // centered exactly at the enemy's world position
            {
                position.x,
                position.y,
                static_cast<float>(texture.width),
                static_cast<float>(texture.height)
            },

            // Rotation origin:
            // center of the texture
            {
                texture.width / 2.0f,
                texture.height / 2.0f
            },

            // Rotation in degrees
            enemy.rotation,

            WHITE
        );
    }

    //Render Allies
    for (size_t i = 0; i < world.allies.size(); i++)
    {
        const Ally& ally = world.allies[i];
        const Position& position = world.allyPositions[i];

        Texture2D texture{};
        switch (ally.type)
        {
        case EnemyType::Basic:
            texture = m_basicEnemyTexture;
            break;

        case EnemyType::Duo:
            texture = m_duoEnemyTexture;
            break;

        case EnemyType::Fast:
            texture = m_fastEnemyTexture;
            break;

        case EnemyType::Tank:
            texture = m_tankEnemyTexture;
            break;
        }
        Rectangle source = {
        0.0f,
        0.0f,
        static_cast<float>(texture.width),
        static_cast<float>(texture.height)
        };

        Rectangle destination = {
            position.value.x,
            position.value.y,
            static_cast<float>(texture.width),
            static_cast<float>(texture.height)
        };

        Vector2 origin = {
            texture.width * 0.5f,
            texture.height * 0.5f
        };

        DrawTexturePro(
            texture,
            source,
            destination,
            origin,
            ally.rotation,
            WHITE
        );

    }

    //Render Powerups
    for (size_t i = 0; i < world.powerups.size(); ++i)
    {
        const Vector2 position = world.powerupPositions[i].value;
        Color color = WHITE;

        Texture2D texture{};
        switch (world.powerups[i].type)
        {
        case PowerupType::DoubleShot:
            texture = m_doubleShotTexture;
            break;

        case PowerupType::TripleShot:
            texture = m_tripleShotTexture;
            break;

        case PowerupType::RapidFire:
            texture = m_rapidFireTexture;
            break;

        case PowerupType::SpeedBoost:
            texture = m_speedBoostTexture;
            break;

        case PowerupType::Shield:
            texture = m_shieldTexture;
            break;

        case PowerupType::Heal:
            texture = m_healthPackTexture;
            break;
        }

        //draw the powerup texture at the powerup's position, centered
        DrawTexture(
            texture,
            static_cast<int>(position.x - texture.width / 2),
            static_cast<int>(position.y - texture.height / 2),
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

void Renderer::DrawProfiler(const Profiler& profiler, float fixedDeltaTime, int simulationSteps)
{
    // Draw the profiler information on the screen
    constexpr int panelWidth = 350;
    constexpr int panelHeight = 455;

    constexpr int margin = 20;
    const int panelX = m_screenWidth - panelWidth - margin;
    const int panelY = m_screenHeight - panelHeight - margin;

    //background panel
    DrawRectangle(panelX, panelY, panelWidth, panelHeight, Fade(BLACK, 0.80f));
    //Draw Border
    DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, WHITE);

    //Title
    DrawText("Profiler", panelX + 15, panelY + 12, 24, SKYBLUE);
    DrawText("F3: Toggle", panelX + panelWidth - 105, panelY + 16, 16, LIGHTGRAY);

    int y = panelY + 52;

    DrawText(
        "FRAME",
        panelX + 15,
        y,
        16,
        LIGHTGRAY
    );

    y += 23;

    DrawText(
        TextFormat(
            "Frame Time     %.2f ms",
            profiler.GetFrameTime()
        ),
        panelX + 15,
        y,
        18,
        WHITE
    );

    y += 25;

    DrawText(
        TextFormat(
            "Simulation     %.2f ms",
            profiler.GetCurrentTime("Simulation")
        ),
        panelX + 15,
        y,
        18,
        WHITE
    );

    y += 25;

    DrawText(
        TextFormat(
            "Rendering      %.2f ms",
            profiler.GetCurrentTime("Render")
        ),
        panelX + 15,
        y,
        18,
        WHITE
    );

    y += 25;

    DrawText(
        TextFormat(
            "Fixed DT       %.2f ms",
            fixedDeltaTime * 1000.0f
        ),
        panelX + 15,
        y,
        18,
        WHITE
    );

    y += 25;

    DrawText(
        TextFormat(
            "Sim Steps      %d",
            simulationSteps
        ),
        panelX + 15,
        y,
        18,
        WHITE
    );

    // -------------------------
    // Systems
    // -------------------------

    y += 32;

    DrawText(
        "SYSTEM",
        panelX + 15,
        y,
        16,
        LIGHTGRAY
    );

    DrawText(
        "CURRENT",
        panelX + 140,
        y,
        16,
        LIGHTGRAY
    );

    DrawText(
        "AVG",
        panelX + 225,
        y,
        16,
        LIGHTGRAY
    );

    DrawText(
        "MAX",
        panelX + 280,
        y,
        16,
        LIGHTGRAY
    );

    y += 24;

    constexpr int rowHeight = 25;

    const char* systemNames[] =
    {
        "Spawn",
        "AI",
        "Weapon",
        "Movement",
        "Projectile",
        "Collision",
        "Powerup",
        "Score",
        "Cleanup"
    };

    for (const char* systemName : systemNames)
    {
        DrawText(
            systemName,
            panelX + 15,
            y,
            18,
            WHITE
        );

        DrawText(
            TextFormat(
                "%.4f",
                profiler.GetCurrentTime(systemName)
            ),
            panelX + 140,
            y,
            18,
            WHITE
        );

        DrawText(
            TextFormat(
                "%.4f",
                profiler.GetAverageTime(systemName)
            ),
            panelX + 225,
            y,
            18,
            WHITE
        );

        DrawText(
            TextFormat(
                "%.4f",
                profiler.GetMaxTime(systemName)
            ),
            panelX + 280,
            y,
            18,
            WHITE
        );

        y += rowHeight;
    }

    // -------------------------
    // Frame Budget
    // -------------------------

    y += 5;

    const float frameBudget = 16.67f;
    const float frameTime = profiler.GetFrameTime();

    DrawText(
        TextFormat(
            "60 FPS Budget: %.2f / %.2f ms",
            frameTime,
            frameBudget
        ),
        panelX + 15,
        y,
        17,
        frameTime > frameBudget ? RED : GREEN
    );
}

// ---------------------------------------------------------
// Basic word wrapping for dialogue
// ---------------------------------------------------------

void Renderer::DrawWrappedText(
    const std::string& text,
    int x,
    int y,
    int maxWidth,
    int fontSize,
    int lineSpacing,
    Color color)
{
    std::istringstream stream(text);

    std::string word;
    std::string currentLine;

    int currentY = y;

    while (stream >> word)
    {
        // Test whether adding this word would exceed the available width.
        std::string testLine =
            currentLine.empty() ? word : currentLine + " " + word;

        if (MeasureText(testLine.c_str(), fontSize) > maxWidth
            && !currentLine.empty())
        {
            // Draw the current line and start a new one.
            DrawText(
                currentLine.c_str(),
                x,
                currentY,
                fontSize,
                color
            );

            currentY += lineSpacing;
            currentLine = word;
        }
        else
        {
            currentLine = testLine;
        }
    }

    // Draw the remaining text.
    if (!currentLine.empty())
    {
        DrawText(
            currentLine.c_str(),
            x,
            currentY,
            fontSize,
            color
        );
    }
}


// ---------------------------------------------------------
// Draw a button
// ---------------------------------------------------------

void Renderer::DrawChoiceButton(
    Rectangle rect,
    const std::string& label,
    Color backgroundColor,
    float alpha)
{
    // Draw the coloured button background.
    DrawRectangleRec(rect, Fade(backgroundColor, alpha));

    // Draw a white outline around the button.
    DrawRectangleLinesEx(rect, 2.0f, Fade(WHITE, alpha));

    // Centre the button label.
    constexpr int fontSize = 24;

    const int textWidth = MeasureText(label.c_str(), fontSize);

    const int textX = static_cast<int>(
        rect.x + (rect.width - textWidth) * 0.5f
        );

    const int textY = static_cast<int>(
        rect.y + (rect.height - fontSize) * 0.5f
        );

    DrawText(
        label.c_str(),
        textX,
        textY,
        fontSize,
        Fade(WHITE, alpha)
    );
}


void Renderer::DrawBackground()
{
    constexpr int tilesize = 256;

    for(int y=0; y<m_screenHeight; y += tilesize)
    {
        for(int x = 0; x < m_screenWidth; x += tilesize)
        {
            DrawTexture(
                m_backgroundTexture,
                x,
                y,
                WHITE
            );
        }
    }
}

void Renderer::DrawCutsceneImposter(EnemyType type, Vector2 position, Vector2 playerPosition)
{   
    Texture2D texture{};
    switch (type)
    {
    case EnemyType::Basic:
        texture = m_basicEnemyTexture;
        break;

    case EnemyType::Duo:
        texture = m_duoEnemyTexture;
        break;

    case EnemyType::Fast:
        texture = m_fastEnemyTexture;
        break;

    case EnemyType::Tank:
        texture = m_tankEnemyTexture;
        break;
    }

    Vector2 dir = Vector2Subtract(playerPosition, position);

    float rot = atan2f(dir.y, dir.x) * RAD2DEG + 90.0f;
    // Destination rectangle places the sprite at the requested world position.
    Rectangle destination = {
        position.x,
        position.y,
        (float)texture.width,
        (float)texture.height
    };

    // Rotate around the centre of the sprite, not its top-left corner.
    Vector2 origin = {
        texture.width * 0.5f,
        texture.height * 0.5f
    };

    Rectangle source = {
        0.0f,
        0.0f,
        (float)texture.width,
        (float)texture.height
    };

    DrawTexturePro(
        texture,
        source,
        destination,
        origin,
        rot,
        WHITE
    );
    
}

void Renderer::DrawCutsceneUI(const DialogueNode& node, DialogueDisplayMode mode, float secondsRemaining)
{
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    float alpha = 1.0f;
    //for final message

    if(mode == DialogueDisplayMode::TimedMessage)
    {
        alpha = Clamp(secondsRemaining / 3.0f, 0.0f, 1.0f);
    }

    if (alpha <= 0.0f)
        return;

    // Main dialogue panel at the bottom of the screen.
    Rectangle panel = {
        screenWidth * 0.08f,
        screenHeight - 295.0f,
        screenWidth * 0.84f,
        285.0f
    };

    DrawRectangleRec(
        panel,
        Fade(BLACK, 0.85f * alpha)
    );

    DrawRectangleLinesEx(
        panel,
        2.0f,
        Fade(WHITE, alpha)
    );

    // Speaker name.
    if (!node.speaker.empty())
    {
        DrawText(
            node.speaker.c_str(),
            static_cast<int>(panel.x + 30),
            static_cast<int>(panel.y + 22),
            26,
            Fade(GOLD, alpha)
        );
    }

    // Dialogue body.
    DrawWrappedText(
        node.dialogue,
        static_cast<int>(panel.x + 30),
        static_cast<int>(panel.y + 68),
        static_cast<int>(panel.width - 60),
        22,
        30,
        Fade(RAYWHITE, alpha)
    );

    // -----------------------------------------------------
    // Node 0: Accept / Reject buttons
    // -----------------------------------------------------
    if (mode == DialogueDisplayMode::Choices)
    {
        if (node.choices.size() >= 2)
        {
            DrawChoiceButton(
                GetChoiceButtonRect(0),
                node.choices[0].text,
                GREEN,
                alpha
            );

            DrawChoiceButton(
                GetChoiceButtonRect(1),
                node.choices[1].text,
                RED,
                alpha
            );
        }

        const char* prompt = "CLICK ACCEPT OR REJECT TO CONTINUE";


        DrawText(
            prompt,
            (screenWidth - MeasureText(prompt, 18)) / 2,
            screenHeight - 38,
            18,
            Fade(LIGHTGRAY, alpha)
        );
    }
    // -----------------------------------------------------
   // Nodes 1 and 2: Spacebar prompt
   // -----------------------------------------------------

    else if (mode == DialogueDisplayMode::Continue)
    {
        const char* prompt = "PRESS SPACE TO CONTINUE";

        DrawText(
            prompt,
            (screenWidth - MeasureText(prompt, 18)) / 2,
            screenHeight - 38,
            18,
            Fade(LIGHTGRAY, alpha)
        );
    }

    // -----------------------------------------------------
    // Nodes 3 and 4: Timed message
    // -----------------------------------------------------

    else if (mode == DialogueDisplayMode::TimedMessage)
    {
        // No buttons or continue prompt.
        // The dialogue panel and its text fade automatically.
    }
}

// ---------------------------------------------------------
// Shared button layout
// ---------------------------------------------------------

Rectangle Renderer::GetChoiceButtonRect(int choiceIndex)
{
    // We only support two choices: Accept (0) and Reject (1).
    if (choiceIndex < 0 || choiceIndex > 1)
        return { 0, 0, 0, 0 };

    constexpr float buttonWidth = 260.0f;
    constexpr float buttonHeight = 60.0f;
    constexpr float gap = 70.0f;

    // Calculate the total width of both buttons and the gap.
    const float totalWidth = buttonWidth * 2.0f + gap;

    // Centre both buttons horizontally.
    const float startX = (GetScreenWidth() - totalWidth) * 0.5f;

    // Place them near the bottom of the dialogue panel.
    const float y = GetScreenHeight() - 110.0f;

    const float x = startX + choiceIndex * (buttonWidth + gap);

    return {
        x,
        y,
        buttonWidth,
        buttonHeight
    };
}
