#pragma once

#include "graphics/core/Texture.hpp"

class SkyBox
{
public:
    SkyBox();
    ~SkyBox();
    SkyBox(const SkyBox& other);
    SkyBox& operator=(const SkyBox& other);    
    SkyBox(SkyBox&& other) noexcept;
    SkyBox& operator=(SkyBox&& other) noexcept;
    
    void clear();
    void create(Image images[6], TextureParams params);

private:
    Texture texture;
};