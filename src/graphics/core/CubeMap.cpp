#include "graphics/core/CubeMap.hpp"

#include "core/Logger.hpp"

#include <glad/glad.h>
#include <stb_image.h>

#ifdef DEBUG_MODE
    int generatedCubeMaps = 0;
#endif

CubeMap::CubeMap() : id_(0) {}
CubeMap::~CubeMap() {glDeleteTextures(1, &id_);}
void CubeMap::create(std::array<std::string, 6> textures_faces) {
    if (id_ != 0) {
        glDeleteTextures(1, &id_);
        id_ = 0;
    }

    glGenTextures(1,&id_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id_);

    int width, height, nrChannels;
    unsigned char *data;
    for(unsigned int i = 0; i < textures_faces.size(); i++)
    {
        data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
            height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else {
            core::logger.log(core::Logger::WARNING, "The image for cubemap wasn't loaded");
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    stbi_image_free(data);

    #ifdef DEBUG_MODE
        generatedCubeMaps++;
        core::logger.log(core::Logger::INFO, "CubeMap " + std::to_string(generatedCubeMaps) + " is generated");
    #endif
}

void CubeMap::activeUnit(int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    unit_ = index;
}
int CubeMap::getUnitId() const {
    return unit_;
}

void CubeMap::bind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, id_);
}