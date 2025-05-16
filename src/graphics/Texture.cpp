#include "graphics/Texture.hpp"
#include "stb_image.h"

#include "Core/Logger.hpp"


Texture::Texture(GLuint id, int width, int height, int nrChannels, GLenum target, std::string path, std::string file_name) : 
    _id(id), 
    _width(width), 
    _height(height), 
    _nrChannels(nrChannels), 
    _target(target),
    _path(path),
    _file_name(file_name) {}


Texture::~Texture() {
    if (_id != 0) {
        glDeleteTextures(1, &_id);
        _id = 0;
    }
}

void Texture::bind() const {
    glBindTexture(_target, _id);
}
void Texture::unbind() const {
    glBindTexture(_target, 0);
}

std::string Texture::getPath() const {
    return _path;
}

std::string Texture::getFileName() const {
    return _file_name;
}




Texture Texture::create(
    std::string path, 
    std::string file_name, 
    TextureParams params
) {
    GLuint id;
    glGenTextures(1, &id);

    glBindTexture(params.target, id);

    glTexParameteri(params.target, GL_TEXTURE_WRAP_S, params.wrap_s);
    glTexParameteri(params.target, GL_TEXTURE_WRAP_T, params.wrap_t);

    if (params.target == GL_TEXTURE_3D) {
        glTexParameteri(params.target, GL_TEXTURE_WRAP_R, params.wrap_r);
    }
    
    if (params.wrap_s == GL_CLAMP_TO_BORDER || 
        params.wrap_t == GL_CLAMP_TO_BORDER || 
        params.wrap_r == GL_CLAMP_TO_BORDER) {
        glTexParameterfv(params.target, GL_TEXTURE_BORDER_COLOR, params.border_color);
    }

    glTexParameteri(params.target, GL_TEXTURE_MAG_FILTER, params.mag_filter);
    glTexParameteri(params.target, GL_TEXTURE_MIN_FILTER, params.min_filter);
    

    int width, height, nrChannels;
    unsigned char* data = stbi_load((path + "/" + file_name).c_str(), &width, &height, &nrChannels, 0);
    

    if (!data) {
        std::string log = "Image at the path: [" + path + "] wasn't loaded";
        logger.log(Logger::WARNING, log);
        
        if (id != 0) {
            glDeleteTextures(1, &id);
        }
        
        return Texture(0, 0, 0, 0, 0, "", "");        
    }
    else {
        GLenum format = GL_RGB;
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;
        else {
            logger.log(Logger::WARNING, "Unknown channel count: " + std::to_string(nrChannels));
            stbi_image_free(data);
            glDeleteTextures(1, &id);
            return Texture(0, 0, 0, 0, 0, "", "");
        }


        switch (params.target) {
        
        default: glTexImage2D(params.target, 0, params.internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data); break;
        }

        glGenerateMipmap(params.target);
    }
    stbi_image_free(data);

    
    return Texture(id, width, height, nrChannels, params.target, path, file_name);
}