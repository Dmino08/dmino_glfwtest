#pragma once


#include <glad/glad.h>

#include <iostream>

struct TextureParams {
    GLenum target = GL_TEXTURE_2D;

    GLenum wrap_s = GL_REPEAT;
    GLenum wrap_t = GL_REPEAT;
    GLenum wrap_r = GL_REPEAT;

    GLenum mag_filter = GL_NEAREST;
    GLenum min_filter = GL_LINEAR;

    float border_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    GLuint internal_format = GL_RGB;
};


class Texture {
    GLuint _id;
    GLenum _target;
    int _width, _height, _nrChannels;
    
    std::string _path;
    std::string _file_name;

    public:
        std::string _type;
        Texture(GLuint id, int width, int height, int nrChannels, GLenum target, std::string path, std::string file_name);
        ~Texture();

        void bind() const;
        void unbind() const;

        std::string getPath() const;
        std::string getFileName() const;


        static Texture create(
            std::string path,
            std::string file_name, 
            TextureParams params = TextureParams()
        );
};
