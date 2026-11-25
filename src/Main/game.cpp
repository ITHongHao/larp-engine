// Starting point of executable
// Currently only starts up the engine with a texture render test
// Empty functions that isn't the constructor will be updated in a future commit for the flappy bird test
#include "game.h"

#include <SDL3/SDL_main.h>
#include <glm/gtc/matrix_transform.hpp>

// Constructor
// Moves width/height to the member variables Width and Height
Game::Game(unsigned int width, unsigned int height) : State(GAME_ACTIVE), Keys{}, Width(width), Height(height) {}

void Game::Init()
{
  m_Resources = std::make_unique<larp::ResourceManager>();

  m_SpriteShader = &m_Resources->LoadShader(
    "shaders/sprite.vert",
    "shaders/sprite.frag",
    "sprite"
  );

  m_Projection = glm::ortho(
    0.0f, static_cast<float>(Width),
    static_cast<float>(Height), 0.0f,
    -1.0f, 1.0f
  );

  m_SpriteShader->Use();
  m_SpriteShader->SetInt("image", 0);
  m_SpriteShader->SetMat4("projection", m_Projection);

  m_MinionTexture = &m_Resources->LoadTexture("./assets/minion.png", "minion");
  m_Renderer = std::make_unique<larp::SpriteRenderer>(*m_SpriteShader);
}

void Game::ProcessInput(float dt)
{

}

void Game::Update(float dt)
{

}

void Game::Render()
{
  if (m_Renderer && m_MinionTexture)
  {
    m_Renderer->DrawSprite(*m_MinionTexture, glm::vec2(200.f, 220.0f), glm::vec2(34.0f, 24.0f), 0.0f);
  }
}

void Game::Shutdown()
{
  m_Renderer.reset();
  m_Resources.reset();
}

int main(int argc, char* argv[])
{
  Game flappyBird(larp::Configuration::width, larp::Configuration::height);
  larp::Engine engine;
  return engine.run_engine(flappyBird);
}