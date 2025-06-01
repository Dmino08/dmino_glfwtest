#include "graphics/core/Model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "core/Logger.hpp"


namespace modload {

    void Model::loadModel(std::string path) {
        
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate
                                                     | aiProcess_FlipUVs 
                                                     | aiProcess_PreTransformVertices
                                                    );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            core::logger.log(core::Logger::ERROR, importer.GetErrorString());
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void Model::processNode(aiNode* node, const aiScene* scene) {

        for (size_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes_.push_back(processMesh(mesh, scene));
        }

        for (size_t i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
        
    }

    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
        
        std::vector<SimpleVertex> vertices;
        std::vector<uint> indices;
        std::vector<Texture> textures;

        for (size_t i = 0; i < mesh->mNumVertices; i++)
        {
            SimpleVertex vertex;
            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;
            
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;

            if (mesh->mTextureCoords[0])
            {
                vertex.uv_coord.x = mesh->mTextureCoords[0][i].x;
                vertex.uv_coord.y = mesh->mTextureCoords[0][i].y;
            }
            else {
                vertex.uv_coord = glm::vec2(0.0f, 0.0f);
            }
            vertices.push_back(vertex);
        }

        for (size_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (size_t j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }
        
        
        // if (mesh->mMaterialIndex >= 0)
        // {
        //     aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        //     std::vector<Texture> diffuseMaps = loadMaterialTextures(material,)
        // }

        Mesh mesh_;
        mesh_.create<SimpleVertex>(MeshData<SimpleVertex>{vertices, indices});
        return mesh_;
    }

    // std::vector<Texture> Model::loadMaterialTextures(aiMaterial& mat, 
    //                                                  aiTextureType type, 
    //                                                  std::string type_name) 
    // {

    // }



    void Model::create(char* path) {
        loadModel(path);
    }

    void Model::draw(GLenum mode) {
        for (size_t i = 0; i < meshes_.size(); i++)
        {
            meshes_[i].draw(mode);
        }
    }

}