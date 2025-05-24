#pragma once

#include <unordered_map>
#include <typeindex>
#include <string>
#include <memory>
#include <optional>

class BaseAsset;

using assets_map = std::unordered_map<std::type_index, std::unordered_map<std::string, std::unique_ptr<BaseAsset>>>;

template<typename T>
constexpr void assert_is_base_asset() {
    static_assert(std::is_base_of<BaseAsset, T>::value, "T must inherit from BaseAsset");
}

class Assets {

    assets_map assets;

    public:
        template<typename T>
        void addItem(const T& item, const std::string& name) {
            assert_is_base_asset<T>();
            assets[std::type_index(typeid(T))][name] = std::make_unique<T>(item);
        }

        template<typename T>
        void DeleteItem(const std::string& name) {
            assert_is_base_asset<T>();

            auto type_it = assets.find(std::type_index(typeid(T)));
            if (type_it == assets.end()) return;

            auto& inner_map = type_it->second;

            auto item_it = inner_map.find(name);
            if (item_it != inner_map.end())
                inner_map.erase(item_it);
        }

        template<typename T>
        std::optional<std::reference_wrapper<T>> getItem(const std::string& name) const {
            assert_is_base_asset<T>();

            auto type_it = assets.find(std::type_index(typeid(T)));
            if (type_it == assets.end()) return std::nullopt;  

            auto& inner_map = type_it->second;

            auto item_it = inner_map.find(name);
            if (item_it == inner_map.end()) return std::nullopt;

            T& ref = *static_cast<T*>(item_it->second.get());

            return std::ref(ref);
        }

        template<typename T>
        bool hasItem(const std::string& name) const {
            assert_is_base_asset<T>();

            auto type_it = assets.find(std::type_index(typeid(T)));
            if (type_it == assets.end()) return false;  

            auto& inner_map = type_it->second;

            return inner_map.find(name) != inner_map.end();
        }


};