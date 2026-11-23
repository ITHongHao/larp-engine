#include "texture2d.h"

// Textures
// Binding, creation, and other actions
namespace larp
{
  Texture2D::Texture2D(unsigned char* data, int width, int height, int channels) 
    : Width(width), Height(height) 
  {
    glGenTextures(1, &ID); // VRAM makes room for [1] texture
    glBindTexture(GL_TEXTURE_2D, ID); // Bind

    // Further code is only on the active texture ID

    // Configure standard wrapping and filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //change GL_LINEAR to something else for pixel-art-based games
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //refer to comment above

    // Determine format based on image channels (3 = RGB, 4 = RGBA)
    unsigned int format = (channels == 4) ? GL_RGBA : GL_RGB;

    // Copies image bytes from RAM to VRAM
    if (data)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, format, Width, Height, 0, format, GL_UNSIGNED_BYTE, data);
    }

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  // Delete
  Texture2D::~Texture2D()
  {
    glDeleteTextures(1, &ID);
  }

  // Bind Texture
  // apparently bind is off convention wise, so naming is altered for standardization here
  void Texture2D::Use() const
  {
    glBindTexture(GL_TEXTURE_2D, ID);
  }

}