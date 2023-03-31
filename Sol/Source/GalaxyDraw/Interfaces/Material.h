#pragma once
#include <glm/glm.hpp>

class Material 
{
public:
    uint32_t textureIndices[5];
    glm::vec4 color; 
    // Other material properties go here
};