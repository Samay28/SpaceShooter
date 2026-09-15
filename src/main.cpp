#include "Game.h"

int main()
{   
    InitAudioDevice();
    Game game;
    game.Run();
    CloseAudioDevice();
    return 0;
}