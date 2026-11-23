#ifndef GAME_H
#define GAME_H

#include <glm/glm.hpp>
#include <memory>

#include "engine.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

// Represents current state of the game
enum GameState
{
  GAME_ACTIVE,
  GAME_MENU,
  GAME_WIN
};

// Holds all states and functionalities of the game
// For easy accessibility
class Game : public larp::GameBase
{
public:
  // Game state
  GameState    State;	
  bool         Keys[1024];
  unsigned int Width, Height;

  // Constructor/Destructor
  Game(unsigned int width, unsigned int height);
  ~Game() override = default;

  // Initialize game state (shaders/textures/etc)
  void Init() override;
  
  // Game loop
  void ProcessInput(float dt) override;
  void Update(float dt) override;
  void Render() override;
  void Shutdown() override;
  
private:
  // Additional Members
  std::unique_ptr<larp::ResourceManager> m_Resources;
  std::unique_ptr<larp::SpriteRenderer> m_Renderer;
  larp::Shader* m_SpriteShader = nullptr;
  larp::Texture2D* m_MinionTexture = nullptr;
  glm::mat4 m_Projection;
};

#endif