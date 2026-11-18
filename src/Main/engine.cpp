#include <glad/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <iostream>

#include "resource_manager.h"
#include "sprite_renderer.h"

//window size
const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;

// Main Function
// Deals with initialization of OpenGL, SDL3, and window setup
int run_engine(int argc, char* argv[])
{
  SDL_Window *window; //main window variable
  bool done = false; //check for window startup

  //starts up sdl library and configures use with OpenGL 3.3.0
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  //basic window config
  window = SDL_CreateWindow(
    "Window",
    SCR_WIDTH,
    SCR_HEIGHT,
    SDL_WINDOW_OPENGL
  );

  //failstate with resolution or errors with graphics drivers
  if (window == NULL)
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
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Inner scope ensures SpriteRenderer and ResourceManager destructors run
  // while the OpenGL context is still alive
  {
    larp::ResourceManager resources;

    larp::Shader& spriteShader = resources.LoadShader(
      "shaders/sprite.vert",
      "shaders/sprite.frag",
      "sprite"
    );

    glm::mat4 projection = glm::ortho(
      0.0f, static_cast<float>(SCR_WIDTH),
      static_cast<float>(SCR_HEIGHT), 0.0f,
      -1.0f, 1.0f
    );

    spriteShader.Use();
    spriteShader.SetInt("image", 0);
    spriteShader.SetMat4("projection", projection);

    larp::Texture2D& minionTexture = resources.LoadTexture("./assets/minion.png", "minion");
    larp::SpriteRenderer renderer(spriteShader);

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
      renderer.DrawSprite(minionTexture, glm::vec2(200.f, 220.0f), glm::vec2(34.0f, 24.0f), 0.0f);
      //renders frame to hidden memory buffer, so viewer won't see computation of elements being drawn in real time. 
      //when drawing is complete, switch hidden buffer with visible one. Just switches the memory address pointer, no reallocation or deleting.
      SDL_GL_SwapWindow(window); 
    }
  }
  //close and destroy window safely
  SDL_GL_DestroyContext(glcontext);
  SDL_DestroyWindow(window); //delete SDL3 window
  SDL_Quit();
  std::cout << "closed\n";
  return 0;
}