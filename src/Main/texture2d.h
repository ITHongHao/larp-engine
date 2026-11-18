#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <glad/gl.h>

// Texture2D can store/configure a texture in OpenGL
// Also contains multiple utility functions
namespace larp
{
  class Texture2D
  {
  public:
    //state
    unsigned int ID = 0;

    //texture dimensions
    int Width = 0, Height = 0;

    //constructor (RAII) / destructor
    Texture2D(unsigned char* data, int width, int height, int channels); //used in stbi_load from stb_image
    ~Texture2D();

    // Prevents copying; transfers GPU texture ownership on move
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;
    Texture2D(Texture2D&& other) noexcept : ID(other.ID), Width(other.Width), Height(other.Height)
    {
      other.ID = 0;
      other.Width = 0;
      other.Height = 0;
    }
    Texture2D& operator=(Texture2D&& other) noexcept
    {
      if (this != &other)
      {
        glDeleteTextures(1, &ID);
        ID = other.ID;
        Width = other.Width;
        Height = other.Height;
        other.ID = 0;
        other.Width = 0;
        other.Height = 0;
      }
      return *this;
    }

    //activates texture for subsequent draw calls
    void Use() const;
  };
}

#endif