#pragma once
#include <array>
#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>

#include "Component.h"
#include <unordered_map>

namespace comp_prop_kit_lib {
    class PropertyList {
    public:
        using HasmMap_Comp = std::unordered_map<size_t, std::unique_ptr<Component>>;

        template<class T>
        T& pushComponent();

        template<class T>
        _NODISCARD T& getComponent();

        template<class T>
        _NODISCARD T getComponent() const;

        std::vector<size_t> getExistingTypeHash() const noexcept;

        template<class T>
        void popComponent();
    private:
        HasmMap_Comp mComponents;
    };

    template<class T>
    T& PropertyList::pushComponent() {
        static_assert(std::is_base_of<Component, T>::value, "assert: template don't have a parent Component");

        mComponents.insert(std::make_pair(typeid(T).hash_code(), std::unique_ptr<Component>{ new T() }));
        mComponents[typeid(T).hash_code()]->init();
        return *static_cast<T*>(mComponents[typeid(T).hash_code()].get());
    }

    template<class T>
    T& PropertyList::getComponent() {
        static_assert(std::is_base_of<Component, T>::value, "assert: template don't have a parent Component");

        auto iter = mComponents.find(typeid(T).hash_code());
        
        if (iter == mComponents.end()) {
            std::cerr << "invalid argument at " << __FUNCTION__ << ": component with this type: \"" << typeid(T).name() << "\" isn't existing\n";
            throw std::invalid_argument("component with this certain type isn't existing");
        }

        return *static_cast<T*>(iter->second.get());
    }

    template<class T>
    T PropertyList::getComponent() const {
        static_assert(std::is_base_of<Component, T>::value, "assert: template don't have a parent Component");

        auto iter = mComponents.find(typeid(T).hash_code());

        if (iter == mComponents.end()) {
            std::cerr << "invalid argument at " << __FUNCTION__ << ": component with this type: \"" << typeid(T).name() << "\" isn't existing\n";
            throw std::invalid_argument("component with this certain type isn't existing");
        }

        return *static_cast<T*>(iter->second.get());
    }

    template<class T>
    void PropertyList::popComponent() {
        static_assert(std::is_base_of<Component, T>::value, "assert: template don't have a parent Component");

        auto iter = mComponents.find(typeid(T).hash_code());

        if (iter == mComponents.end()) {
            std::cerr << "invalid argument at " << __FUNCTION__ << "t: component with this type: \"" << typeid(T).name() << "\" isn't existing\n";
            throw std::invalid_argument("component with this certain type isn't existing");
        }

        mComponents.erase(iter);
    }
}

