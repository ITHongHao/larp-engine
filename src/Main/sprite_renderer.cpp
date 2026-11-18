#include "sprite_renderer.h"

namespace larp
{
  SpriteRenderer::SpriteRenderer(const Shader& shader)
    : m_Shader(shader)
  {
    initRenderData();
  }

  SpriteRenderer::~SpriteRenderer()
  {
    glDeleteVertexArrays(1, &m_QuadVAO);
    glDeleteBuffers(1, &m_QuadVBO);
  }

  void SpriteRenderer::initRenderData()
  {
    // 2 triangles forming a [0, 1] unit quad: (pos.x, pos.y, uv.x, uv.y)
    float vertices[] = {
      // pos      // tex
      0.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f,

      0.0f, 1.0f, 0.0f, 1.0f,
      1.0f, 1.0f, 1.0f, 1.0f,
      1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &m_QuadVAO);
    glGenBuffers(1, &m_QuadVBO);

    // bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_QuadVAO);
    glEnableVertexAttribArray(0);
    //listen to location 0
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    //glVertexAttribPointer already made a permanent link to VBO and VAO. Safe to unbind VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //as a precaution, unbind so other VAO calls won't modify this VAO by accident.
    glBindVertexArray(0);
  }

  void SpriteRenderer::DrawSprite(const Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) const
  {
    // setup transformations
    m_Shader.Use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    m_Shader.SetMat4("model", model);
    m_Shader.SetVec3("spriteColor", color); // render textured quad

    glActiveTexture(GL_TEXTURE0);
    texture.Use();

    glBindVertexArray(m_QuadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
  }
}