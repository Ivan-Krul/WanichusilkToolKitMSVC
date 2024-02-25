#pragma once
#include <vector>
#include <memory>
#include <iostream>

#include <raylib.h>

#include "GraphicElement.h"
#include "GraphicElementFactory.h"

#include "Hardware.h"

namespace graphic_sys_lib {
    class Renderer 
    : public ParseHardwareHelper {
    public:
        virtual void render();
        void renderBase(const std::vector<std::unique_ptr<GraphicElement>>& elements) const noexcept;

        // STL interface for interacting with mElements
        void clear() noexcept { mElements.clear(); }
        template<typename T, typename... TArgs>
        inline void push(TArgs&&... m_args);
        inline _NODISCARD size_t size() const noexcept { return mElements.size(); }
        
        template<typename T>
        inline _NODISCARD T& element(size_t index);

        template<typename T>
        inline _NODISCARD const T at(size_t index) const;

        inline _NODISCARD std::vector<std::unique_ptr<GraphicElement>>::iterator begin() noexcept { return mElements.begin(); }
        inline _NODISCARD std::vector<std::unique_ptr<GraphicElement>>::iterator end() noexcept { return mElements.end(); }
        inline _NODISCARD std::vector<std::unique_ptr<GraphicElement>>::const_iterator cbegin() const noexcept { return mElements.cbegin(); }
        inline _NODISCARD std::vector<std::unique_ptr<GraphicElement>>::const_iterator cend() const noexcept { return mElements.cend(); }
        inline _NODISCARD std::vector<std::unique_ptr<GraphicElement>>::reverse_iterator rbegin() noexcept { return mElements.rbegin(); }
        inline _NODISCARD std::vector<std::unique_ptr<GraphicElement>>::reverse_iterator rend() noexcept { return mElements.rend(); }
        inline _NODISCARD std::vector<std::unique_ptr<GraphicElement>>::const_reverse_iterator crbegin() const noexcept { return mElements.crbegin(); }
        inline _NODISCARD std::vector<std::unique_ptr<GraphicElement>>::const_reverse_iterator crend() const noexcept { return mElements.crend(); }

        template<typename ...Types>
        void parseFile(const std::string& pathToCMPT);

        _NODISCARD bool isParsableFile(const std::string& pathToCMPT);

        virtual ~Renderer() = default;
    protected:
        std::vector<std::unique_ptr<GraphicElement>> mElements;
    };

    template<typename T, typename ...TArgs>
    inline void Renderer::push(TArgs && ...m_args) {
        static_assert(std::is_base_of<GraphicElement, T>::value, "assert: template isn't based on GraphicElement");

        T* element = new T(std::forward<TArgs>(m_args)...);
        std::unique_ptr<GraphicElement>uPtr{element};
        mElements.push_back(std::move(uPtr));
    }

    template<typename T>
    inline T& Renderer::element(size_t index) {
        if (index >= mElements.size()) {
            std::cerr << "out of range: index to GraphicElement is overflew\n";
            throw std::out_of_range("index to GraphicElement is overflew");
        }

        return *static_cast<T*>(mElements[index].get());
    }

    template<typename T>
    inline const T Renderer::at(size_t index) const {
        if (index >= mElements.size()) {
            std::cerr << "out of range: index to GraphicElement is overflew\n";
            throw std::out_of_range("index to GraphicElement is overflew");
        }

        return *static_cast<T*>(*mElements.at(index));
    }

    template<typename ...Types>
    inline void Renderer::parseFile(const std::string& pathToCMPT) {
        auto& compacter = parseCmptFile(pathToCMPT);

        GraphicElementFactory geFactory;
        geFactory.parseFloatVarArr(compacter["graphic_elements"], GraphicElementFactory::TypeList<Types...>());
    }
}
