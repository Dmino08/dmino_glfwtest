#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

#include "graphics/core/Texture.hpp"
#include "graphics/core/Mesh.hpp"
#include "graphics/glsl/GLSLStructures.hpp"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;



namespace modload { 

    enum TextureType {
        DIFFUSE,
        SPECULAR
    };

    struct ModelMesh {
        Mesh mesh;
        int material_index;
    };

    class Model {
        std::vector<ModelMesh> meshes_;
        std::vector<glsl::Material> materials_;
        std::unordered_map<std::string, int> texture_cache_;
        std::vector<Texture> textures_flat_;
        
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        ModelMesh processMesh(aiMesh* mesh, const aiScene* scene);
        int Model::loadMaterialTexture(aiMaterial& mat, aiTextureType type);

        public:
            void create(char* path);
            void draw(GLenum mode = GL_TRIANGLES);

            std::vector<ModelMesh>& getMeshes();
            std::vector<glsl::Material>& getMaterials();
            std::vector<Texture>& getAllTextures();
    };
}