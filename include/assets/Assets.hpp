#pragma once

#include <unordered_map>
#include <typeindex>
#include <string>
#include <memory>
#include <optional>


using assets_map = std::unordered_map<std::type_index, std::unordered_map<std::string, std::shared_ptr<void>>>;

template<typename type>
using asset = std::shared_ptr<type>;

class Assets {

    assets_map assets;

    public:
        template<typename T>
        void addItem(const std::shared_ptr<T>& item, const std::string& name) {

            assets[std::type_index(typeid(T))][name] = item;
        }

        template<typename T>
        void DeleteItem(const std::string& name) {

            auto type_it = assets.find(std::type_index(typeid(T)));
            if (type_it == assets.end()) return;

            auto& inner_map = type_it->second;

            auto item_it = inner_map.find(name);
            if (item_it != inner_map.end())
                inner_map.erase(item_it);
        }

        template<typename T>
        std::shared_ptr<T> getItem(const std::string& name) const {

            auto type_it = assets.find(std::type_index(typeid(T)));
            if (type_it == assets.end()) return nullptr;  

            auto& inner_map = type_it->second;

            auto item_it = inner_map.find(name);
            if (item_it == inner_map.end()) return nullptr;

            return std::static_pointer_cast<T>(item_it->second);
        }

        template<typename T>
        bool hasItem(const std::string& name) const {

            auto type_it = assets.find(std::type_index(typeid(T)));
            if (type_it == assets.end()) return false;  

            auto& inner_map = type_it->second;

            return inner_map.find(name) != inner_map.end();
        }


};