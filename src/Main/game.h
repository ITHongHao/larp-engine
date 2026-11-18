#ifndef GAME_H
#define GAME_H

#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

// Represents current state of the game
enum GameState
{
  GAME_ACTIVE,
  GAME_MENU,
  GAME_WIN
};

// Holds all states and functionalities of the game
// For easy accessibility
class Game
{
  public:
    // game state
    GameState    State;	
    bool         Keys[1024];
    unsigned int Width, Height;

    Game(unsigned int width, unsigned int height);
    ~Game();

    // initialize game state (shaders/textures/etc)
    void Init();
    
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
};

#endif