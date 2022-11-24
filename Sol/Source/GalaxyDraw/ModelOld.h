#ifndef MODEL_H
#define MODEL_H

#include "MeshOld.h"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

namespace GalaxyDraw {
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

	class ModelOld
	{
	public:
		std::vector<MeshTexture> textures_loaded;
		std::vector<MeshOld> meshes;
		std::string directory;
		bool gammaCorrection;

		ModelOld(std::string const& path, bool gamma = false);
		void Draw(Shader& shader);

	private:
		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		MeshOld processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};
}
#endif
