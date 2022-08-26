#pragma once

#include <glad/glad.h>
#include "VAO.h"
#include "EBO.h"
#include "shaderClass.h"
#include "Model.h"

class RenderScreen
{
public:
    void Draw(const VAO& va, const EBO& ib, const Shader& shader) const;
    void Draw(const Model& model, const Shader& shader) const;
    void Clear() const;
};