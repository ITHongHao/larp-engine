// Resource Manager

#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "stb_image.h"

namespace larp {
  // Constructor already has default in header file
  // Destructor
  ResourceManager::~ResourceManager()
  {
    Clear();
  }

  // Load Shader
  Shader& ResourceManager::LoadShader(const std::string& vShaderFile, const std::string& fShaderFile, const std::string& name)
  {
    auto [it, inserted] = m_Shaders.insert_or_assign(name, ResourceManager::LoadShaderFromFile(vShaderFile, fShaderFile));
    return it->second;
  }

  // Get Shader
  Shader& ResourceManager::GetShader(const std::string& name)
  {
    return m_Shaders.at(name);
  }

  // Load Texture
  Texture2D& ResourceManager::LoadTexture(const std::string& file, const std::string& name)
  {
    auto [it, inserted] = m_Textures.insert_or_assign(name, ResourceManager::LoadTextureFromFile(file));
    return it->second;
  }

  // Get Texture
  Texture2D& ResourceManager::GetTexture(const std::string& name)
  {
    return m_Textures.at(name);
  }

  // Clear existing shaders and textures
  void ResourceManager::Clear()
  {
    m_Shaders.clear();
    m_Textures.clear();
  }

  Shader ResourceManager::LoadShaderFromFile(const std::string& vShaderFile, const std::string& fShaderFile)
  {
    // Retrive vertex/fragment code from filepath
    std::string vertexCode;
    std::string fragmentCode;
    try
    {
      // open files
      std::ifstream vertexShaderFile(vShaderFile);
      std::ifstream fragmentShaderFile(fShaderFile);
      vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      std::stringstream vShaderStream, fShaderStream;
      // read file's buffer contents into streams
      vShaderStream << vertexShaderFile.rdbuf();
      fShaderStream << fragmentShaderFile.rdbuf();
      // close file handlers
      vertexShaderFile.close();
      fragmentShaderFile.close();
      // convert stream into string
      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
    }
    catch (const std::exception& e)
    {
      std::cout << "ERROR::SHADER: Failed to read shader files. " << e.what() << std::endl;
    }
    // create shader object
    return Shader(vertexCode, fragmentCode);
  }

  Texture2D ResourceManager::LoadTextureFromFile(const std::string& file)
  {
    // Load image
    int width, height, numChannels;
    unsigned char* data = stbi_load(file.c_str(), &width, &height, &numChannels, 0);
    if (!data)
    {
      std::cout << "ERROR::TEXTURE: Failed to reach texture data." << std::endl;
    }
    // Create/Generate texture,
    Texture2D texture(data, width, height, numChannels);
    // Free image data
    stbi_image_free(data);
    return texture;
  }
}