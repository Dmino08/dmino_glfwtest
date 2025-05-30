#pragma once

#include <vector>
#include <string>
#include <memory>

class Mesh;

namespace modload {
    struct Texture {
        unsigned int id;
    };
    
    struct Material {
        modload::Texture diffuse;
        modload::Texture specular;
    };

    struct Drawable {
        std::unique_ptr<Mesh> mesh;
        std::unique_ptr<modload::Material> material;
    };

    class Model {
        std::vector<modload::Drawable> meshes_;
        

        public:
            // void create(const std::string& path);
    };
}