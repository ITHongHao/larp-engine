// Header file for the engine

#ifndef ENGINE_H
#define ENGINE_H

namespace larp
{
  // Currently class is for window detail setup (width, height)
  class Configuration
  {
  public:
    // Window sizes
    // Immutable compile time variables
    static constexpr unsigned int width = 640;
    static constexpr unsigned int height = 480;
  };

  // Game object
  // Pure Interface
  class GameBase
  {
  public:
    // Refer to the sub classes that uses these functions (eg. game.h)
    virtual ~GameBase() = default;

    virtual void Init() = 0;
    virtual void ProcessInput(float dt) = 0;
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void Shutdown() = 0;
  };

  // Running the engine with the game(base) object
  class Engine
  {
  public:
    // Returns 0 if successful, 1 otherwise
    int run_engine(GameBase& game);
  };
}

#endif