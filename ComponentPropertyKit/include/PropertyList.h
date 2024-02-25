#pragma once
#include <array>
#include <vector>
#include <memory>
#include <stdexcept>
#include <iostream>

#include "Component.h"

namespace comp_prop_kit_lib {
    class PropertyList {
    public:
        template<class T>
        T& pushComponent();

        template<class T>
        _NODISCARD T& getComponent(size_t index);

        template<class T>
        _NODISCARD T getComponent(size_t index) const;

        template<class T>
        void popComponent(size_t index);

    private:
        std::vector<std::shared_ptr<Component>> mComponents;
    };

    template<class T>
    inline T& PropertyList::pushComponent() {
        static_assert(std::is_base_of<Component, T>::value, "assert: template don't have a parent Component");

        T* c = new T(*this, mComponents.size(), typeid(T).hash_code());
        std::shared_ptr<Component> uPtr{c};
        mComponents.push_back(uPtr);
        mComponents.back()->init();
        return *static_cast<T*>(mComponents.back().get());
    }

    template<class T>
    inline T& PropertyList::getComponent(size_t index) {
        static_assert(std::is_base_of<Component, T>::value, "assert: template don't have a parent Component");

        if (index >= mComponents.size()) {
            std::cerr << "out of range: component index is out of range\n";
            throw std::out_of_range("index is overflowing");
        }

        auto comp = [&index](const std::shared_ptr<Component>& ptr) {
            if (ptr->getInfoHash() == typeid(T).hash_code()) {
                if (index > 0) {
                    index--;
                    return false;
                }
                else
                    return true;
            } else
                return false;
        };
        auto iter = std::find_if(mComponents.begin(), mComponents.end(), comp);

        return *static_cast<T*>(iter->get());
    }

    template<class T>
    inline T PropertyList::getComponent(size_t index) const {
        static_assert(std::is_base_of<Component, T>::value, "assert: template don't have a parent Component");

        if (index >= mComponents.size()) {
            std::cerr << "out of range: component index is out of range\n";
            throw std::out_of_range("index is overflowing");
        }

        auto comp = [&index](const std::shared_ptr<Component>& ptr) {
            if (ptr->getInfoHash() == typeid(T).hash_code()) {
                if (index > 0) {
                    index--;
                    return false;
                }
                else
                    return true;
            } else
                return false;
        };
        auto iter = std::find_if(mComponents.begin(), mComponents.end(), comp);

        return *static_cast<T*>(iter->get());
    }

    template<class T>
    inline void PropertyList::popComponent(size_t index) {
        static_assert(std::is_base_of<Component, T>::value, "assert: template don't have a parent Component");

        if (index >= mComponents.size()) {
            std::cerr << "out of range: component index is out of range\n";
            throw std::out_of_range("index is overflowing");
        }

        auto comp = [&index](const std::shared_ptr<Component>& ptr) {
            if (ptr->getInfoHash() == typeid(T).hash_code()) {
                if (index > 0) {
                    index--;
                    return false;
                } else
                    return true;
            } else
                return false;
        };
        auto iter = std::find_if(mComponents.begin(), mComponents.end(), comp);

        mComponents.erase(iter);
    }
}

