#pragma once

class GameWorld;
class Renderer
{
public:
    Renderer(int screenWidth, int screenHeight, const char* title);
    ~Renderer();

    void BeginFrame();
    void EndFrame();

    bool ShouldClose() const;

    void Render(const GameWorld& world);

private:
    int m_screenWidth;
    int m_screenHeight;
};