#include "engine.h"

#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <iostream>

// Main Function
// Deals with initialization of OpenGL, SDL3, and window setup
int larp::Engine::run_engine(GameBase& game)
{
  SDL_Window *window = nullptr; //main window variable
  bool done = false; //check for window startup

  //starts up sdl library and configures use with OpenGL 3.3.0
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  //basic window config
  window = SDL_CreateWindow(
    "Window",
    larp::Configuration::width,
    larp::Configuration::height,
    SDL_WINDOW_OPENGL
  );

  //failstate with resolution or errors with graphics drivers
  if (window == nullptr)
  {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    return 1;
  }

  //state machine for OpenGL that binds to window
  SDL_GLContext glcontext = SDL_GL_CreateContext(window);

  //failstate for if GLAD (loading library) fails to load OpenGL functions from within graphics drivers
  if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, larp::Configuration::width, larp::Configuration::height);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  game.Init();

  //MAIN GAME LOOP
  while (!done)
  {
    //every frame, the OS sends events to the window.
    SDL_Event event;
    //SDL_PollEvent pulls the oldest event out of the event queue.
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_EVENT_QUIT) //eg. 'X' button of a window
      {
        done = true;
      }
    }
    //game logic, present frame, etc
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    game.Render();
    //renders frame to hidden memory buffer, so viewer won't see computation of elements being drawn in real time. 
    //when drawing is complete, switch hidden buffer with visible one. Just switches the memory address pointer, no reallocation or deleting.
    SDL_GL_SwapWindow(window); 
  }
  //close and destroy window safely
  game.Shutdown();
  SDL_GL_DestroyContext(glcontext);
  SDL_DestroyWindow(window); //delete SDL3 window
  SDL_Quit();
  std::cout << "closed\n";
  return 0;
}