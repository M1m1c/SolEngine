#pragma once
#include "Sol/Core/KeyedVector.h"
#include "Model.h"

#include <string>
#include <memory>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace GalaxyDraw 
{
    class ModelManager
    {
    public:
        void DiscardModel(const std::string& path);
        std::shared_ptr<Model> ProcessModel(const std::string& path);
        std::shared_ptr<Model> GetModel(const std::string& path);

        ModelManager() = default;
        ~ModelManager() = default;
        ModelManager(const ModelManager&) = delete;
        ModelManager& operator=(const ModelManager&) = delete;
    private:

        void ProcessNode(aiNode* node, const aiScene* scene, std::vector<std::shared_ptr<Mesh>>& meshes);
        std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene, aiNode* node);
    private:
        KeyedVector<std::string, std::shared_ptr<Model>> m_LoadedModels;

        
    };
}