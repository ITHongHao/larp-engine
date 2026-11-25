#include "shader.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

// Shader:
// Binding, creation, error checks, and other actions
namespace larp
{
  Shader::Shader(std::string_view vertexShaderSource, std::string_view fragmentShaderSource)
  {
    const char* vShaderCode = vertexShaderSource.data();
    const char* fShaderCode = fragmentShaderSource.data();
    const unsigned int characterLogCount = 512;

    unsigned int vertexShader, fragmentShader;
    int success; //1=works, 0=doesn't
    char infoLog[characterLogCount]; //error data storage

    // Build and compile shader program

    // Vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment Shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders and setup shaderprogram object
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    { // Error usually happens when compiling shaders are successful but cannot link together.
      glGetProgramInfoLog(ID, 512, NULL, infoLog);
      std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete compiled shaders to free memory
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
  }

  // Delete
  Shader::~Shader()
  {
    glDeleteProgram(ID);
  }

  // Make Shader active
  void Shader::Use() const
  {
    glUseProgram(ID);
  }

  // Tells shader which texture unit to sample
  void Shader::SetInt(const char* name, int value) const
  {
    glUniform1i(glGetUniformLocation(ID, name), value);
  }

  // Passes RGB color to tint sprite
  void Shader::SetVec3(const char* name, const glm::vec3& value) const
  {
    glUniform3fv(glGetUniformLocation(ID, name), 1, &value[0]);
  }

  // Passes model/projection matrices to place sprite
  void Shader::SetMat4(const char* name, const glm::mat4& matrix) const
  {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
  }

}