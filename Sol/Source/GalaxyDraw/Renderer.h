#pragma once

#include <glad/glad.h>
#include "VAO.h"
#include "EBO.h"
#include "shaderClass.h"

class Renderer
{
public:
    const VAO& VArrayObject; 
    const EBO& IndexBufferObject;

    Renderer(const VAO& va, const EBO& ib);
    void Draw(const Shader& shader) const;
    void Clear() const;
};