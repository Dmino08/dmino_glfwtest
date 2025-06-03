#include "graphics/core/CubeMap.hpp"

#include "core/Logger.hpp"


#include "graphics/core/Image.hpp"

#ifdef DEBUG_MODE
    int generatedCubeMaps = 0;
#endif

CubeMap::CubeMap() : id_(0) {}
CubeMap::~CubeMap() {glDeleteTextures(1, &id_);}
void CubeMap::create(std::array<std::string, 6> textures_faces, GLint internalformat) {
    if (id_ != 0) {
        glDeleteTextures(1, &id_);
        id_ = 0;
    }

    glGenTextures(1,&id_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id_);

    Image image;
    for(unsigned int i = 0; i < textures_faces.size(); i++)
    {
        image.load(textures_faces[i].c_str());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalformat, image.getWidth(),
        image.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.getData());
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);


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