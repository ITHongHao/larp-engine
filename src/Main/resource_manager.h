// Resource Manager Header file

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <unordered_map>
#include <string>

#include "shader.h"
#include "texture2d.h"

namespace larp
{
  class ResourceManager
  {
  public:
    // Constructor/Destructor 
    ResourceManager() = default;
    ~ResourceManager();

    // Prevents copies and accidental memory moving from the ResourceManager
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    // Loads shaders, returns reference to the new shader object
    Shader& LoadShader(const std::string& vShaderFile, const std::string& fShaderFile, const std::string& name);
    
    // Use "name" to get the reference of a previous active shader from m_Shaders map
    // Don't need file path to shader with this
    Shader& GetShader(const std::string& name);

    // Uses stb_image to load image/texture
    // Generates and maps OpenGL texture
    Texture2D& LoadTexture(const std::string& file, const std::string& name);
    
    // Use "name" to get the reference of a previous active texture from m_Textures map
    // Don't need file path to texture with this
    Texture2D& GetTexture(const std::string& name);

    // Deletes textures and shaders from m_Shaders and m_Textures (glDeleteProgram/glDeleteTextures)
    void Clear();

  private:
    // key = string name, val = OpenGL wrapper object
    std::unordered_map<std::string, Shader> m_Shaders;
    std::unordered_map<std::string, Texture2D> m_Textures;

    // Private helper functions to handle the actual file I/O and API calls
    Shader LoadShaderFromFile(const std::string& vShaderFile, const std::string& fShaderFile);
    Texture2D LoadTextureFromFile(const std::string& file);
  };
}

#endif