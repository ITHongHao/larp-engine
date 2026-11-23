#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <string_view>

#include <glad/gl.h>
#include <glm/glm.hpp>

//General shader object
//Compiles from file, generates error messages and contains multiple utility functions
namespace larp
{
  class Shader
  {
  public:
    //state
    unsigned int ID = 0;

    //constructor (RAII) / destructor
    Shader(std::string_view vertexSource, std::string_view fragmentSource);
    ~Shader();

    // Prevents copying; transfers GPU program ownership on move
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept : ID(other.ID)
    {
      other.ID = 0;
    }
    Shader& operator=(Shader&& other) noexcept
    {
      if (this != &other)
      {
        glDeleteProgram(ID);
        ID = other.ID;
        other.ID = 0;
      }
      return *this;
    }

    //activates shader program for subsequent draw calls
    void Use() const;

    //generic utility setter functions
    void SetInt(const char* name, int value) const; //tells shader which texture unit to sample
    void SetVec3(const char* name, const glm::vec3& value) const; //passes RGB color to tint sprite
    void SetMat4(const char* name, const glm::mat4& matrix) const; //passes model/projection matrices to place sprite

  private:
    void checkCompileErrors(unsigned int object, std::string type);
  };
}

#endif