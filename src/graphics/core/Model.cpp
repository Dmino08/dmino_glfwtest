#include "graphics/core/Model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "core/Logger.hpp"
#include "graphics/core/Image.hpp"
#include "graphics/core/VertexStructures.hpp"


namespace modload {

    void Model::loadModel(std::string path) {
        
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate
                                                    //   | aiProcess_FlipUVs 
                                                    );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            core::logger.log(core::Logger::ERROR, importer.GetErrorString());
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);


        for (size_t i = 0; i < scene->mNumMaterials; i++)
        {
            aiMaterial* mat = scene->mMaterials[i];
        
            float shininess = 0.0f;
            mat->Get(AI_MATKEY_SHININESS, shininess);

            int diffuse_id = loadMaterialTexture(*mat, aiTextureType_DIFFUSE);
            int specular_id = loadMaterialTexture(*mat, aiTextureType_SPECULAR);

            glsl::Material material;
            material.diffuse = diffuse_id;
            material.specular = specular_id;
            material.shininess = shininess;

            materials_.push_back(std::move(material));
        }

        #ifdef MODEL_LOGGING
            core::logger.log(core::Logger::INFO, "Model vertex amount: " + std::to_string(loaded_vertices_));
        #endif

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

    ModelMesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
        
        std::vector<SimpleVertex> vertices;
        std::vector<uint> indices;

        #ifdef MODEL_LOGGING
            loaded_vertices_ += mesh->mNumVertices;
        #endif

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
        
        int material_index = mesh->mMaterialIndex;
        Mesh mesh_;
        mesh_.create(vertices.size(), indices.size());
        mesh_.bind();
        mesh_.setBuffer(GL_ARRAY_BUFFER, vertices.size() * sizeof(SimpleVertex), vertices.data(), GL_STATIC_DRAW);
        mesh_.setBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), indices.data(), GL_STATIC_DRAW);

        mesh_.setAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex),
                       reinterpret_cast<void*>(offsetof(SimpleVertex, position)));

        mesh_.setAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex),
                       reinterpret_cast<void*>(offsetof(SimpleVertex, normal)));

        mesh_.setAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex),
                       reinterpret_cast<void*>(offsetof(SimpleVertex, uv_coord)));
        mesh_.unbind();

        return ModelMesh{std::move(mesh_), material_index};
    }

    int Model::loadMaterialTexture(aiMaterial& mat, aiTextureType type) {
        if (mat.GetTextureCount(type) > 0) {
            aiString name_str;
            mat.GetTexture(type, 0, &name_str);

            std::string full_path = directory + "/" + name_str.C_Str();

            // Проверка кеша:
            auto it = texture_cache_.find(full_path);
            if (it != texture_cache_.end())
                return it->second;

            // Загрузка новой текстуры:
            Image image;
            image.load(full_path);

            Texture texture;
            texture.create(image, texture_params);

            int tex_id = textures_flat_.size();
            textures_flat_.push_back(std::move(texture));
            texture_cache_[full_path] = tex_id;

            return tex_id;
        }

        return -1; // если текстуры нет
    }

    void Model::create(char* path) {
        loadModel(path);
    }

    void Model::draw(GLenum mode) {
        for (size_t i = 0; i < meshes_.size(); i++)
        {
            meshes_[i].mesh.draw(mode);
        }
    }

    std::vector<ModelMesh>& Model::getMeshes() {
        return meshes_;
    }

    std::vector<glsl::Material>& Model::getMaterials() {
        return materials_;
    }

    std::vector<Texture>& Model::getAllTextures() {
        return textures_flat_;
    }
}