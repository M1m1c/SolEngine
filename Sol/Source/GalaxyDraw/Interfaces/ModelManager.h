#pragma once
#include "Sol/Core/KeyedVector.h"
#include "GalaxyDraw/Model.h"

#include <string>
#include <memory>

struct aiNode;
struct aiScene;
struct aiMesh;

namespace GalaxyDraw 
{

    struct ModelInstanceData
    {
        ModelInstanceData(uint32_t count, std::shared_ptr<Model> model): Count(count),Model(model){}
        uint32_t Count;
        std::shared_ptr<Model> Model;
    };

    static class ModelManager
    {
    public:
        static void DiscardModelInstance(const std::string& path);
        static std::shared_ptr<Model> ProcessModel(const std::string& path);
        static std::shared_ptr<Model> GetModel(const std::string& path);
        static bool IsModelLoaded(const std::string& path);
        static ModelManager& GetInstance() // static method to get the instance
        {
            static ModelManager instance; // static instance of the class
            return instance;
        }

        ModelManager(const ModelManager&) = delete;
        ModelManager& operator=(const ModelManager&) = delete;

    private:
        ModelManager() = default; // private constructor

        static void ProcessNode(aiNode * node, const aiScene * scene, std::vector<std::shared_ptr<Mesh>>&meshes);
        static std::shared_ptr<Mesh> ProcessMesh(aiMesh * mesh, const aiScene * scene, aiNode * node);

    private:
        KeyedVector < std::string, ModelInstanceData> m_LoadedModels;
    };
}