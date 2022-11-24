#include "solpch.h"
#include "OpenGL_Model.h"

namespace GalaxyDraw
{
    OpenGL_Model::OpenGL_Model(const std::string& modelpath)
    {
    }

    void OpenGL_Model::SetData(const std::string& path)
    {
    }

    bool OpenGL_Model::operator==(const Model& other) const
    {
        return false;
    }

    std::vector<Mesh> OpenGL_Model::GetMeshes() const
    {
        return std::vector<Mesh>();
    }

}

