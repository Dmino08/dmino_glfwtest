#pragma once

#include <vector>
#include <string>
#include <memory>

#include "graphics/core/Texture.hpp"
#include "graphics/core/Mesh.hpp"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;


namespace modload { 

    class Model {
        std::vector<Mesh> meshes_;
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        // std::vector<Texture> loadMaterialTextures(aiMaterial& mat, 
        //                                           aiTextureType type, 
        //                                           std::string type_name);

        public:
             void create(char* path);
             void draw(GLenum mode = GL_TRIANGLES);
    };
}