#pragma once

#include "graphics/core/Texture.hpp"
#include <string>

constexpr TextureParams SKYBOX_PARAMS = 
{
    GL_TEXTURE_CUBE_MAP,

    GL_CLAMP_TO_EDGE,
    GL_CLAMP_TO_EDGE,
    GL_CLAMP_TO_EDGE,

    GL_LINEAR,
    GL_LINEAR,

    GL_UNSIGNED_BYTE,

    {1.0f, 1.0f, 1.0f, 1.0f},

    GL_RGB,
    GL_RGB
};

class SkyBox
{
public:
    SkyBox();
    ~SkyBox();
    SkyBox(const SkyBox& other) = delete; 
    SkyBox& operator=(const SkyBox& other) = delete;    
    SkyBox(SkyBox&& other) noexcept;
    SkyBox& operator=(SkyBox&& other) noexcept;
    
    void clear();
    void create(Image images[6], const TextureParams& params);
    void create(std::string paths[6], const TextureParams& params);

    Texture texture;
};