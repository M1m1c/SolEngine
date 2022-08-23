#include "Renderer.h"

void Renderer::Draw(const Shader& shader) const
{
    shader.Bind();
    VArrayObject.Bind();
    IndexBufferObject.Bind();

    glDrawElements(GL_TRIANGLES, IndexBufferObject.count, GL_UNSIGNED_INT, nullptr);
}

Renderer::Renderer(const VAO& va, const EBO& ib) : VArrayObject(va), IndexBufferObject(ib)
{
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}