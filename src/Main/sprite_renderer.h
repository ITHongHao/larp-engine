#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "texture2d.h"

namespace larp
{
  class SpriteRenderer
  {
  public:
    // Constructor/Destructor
    explicit SpriteRenderer(const Shader& shader);
    ~SpriteRenderer();

    // Prevents copies and accidental memory moving from the SpriteRenderer
    SpriteRenderer(const SpriteRenderer&) = delete;
    SpriteRenderer& operator=(const SpriteRenderer&) = delete;
    SpriteRenderer(SpriteRenderer&&) = delete;
    SpriteRenderer& operator=(SpriteRenderer&&) = delete;

    // Renders a defined quad textured with the given sprite
    void DrawSprite(const Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f)) const;

  private:
    // Render State
    const Shader& m_Shader;
    unsigned int m_QuadVAO = 0;
    unsigned int m_QuadVBO = 0;

    // Initializes buffer/vertex from the quad
    void initRenderData();
  };
}

#endif