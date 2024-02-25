#pragma once
#include "FloatVarBaseExpander.h"

#include <unordered_map>
#include <initializer_list>

namespace float_var_container_lib {
    class FloatVarObjectList :
        public FloatVarBaseExpander {
    public:
        FloatVarObjectList();
        FloatVarObjectList(const FloatVarObjectList& inst);
        FloatVarObjectList(const std::vector<FloatVar>& float_vars) noexcept;
        FloatVarObjectList(std::initializer_list<FloatVar> init_list);
        FloatVarObjectList(const std::vector<FloatVarBaseExpander>& init_list);
        FloatVarObjectList(std::initializer_list<FloatVarBaseExpander> init_list);
        FloatVarObjectList(FloatVarObjectList&& inst) noexcept;
        FloatVarObjectList(std::vector<FloatVar>&& float_vars) noexcept;
        virtual ~FloatVarObjectList() {}

        void operator= (const FloatVarObjectList& float_vars) noexcept;
        void operator= (FloatVarObjectList&& float_vars) noexcept;
        void operator= (const std::vector<FloatVar>& float_vars) noexcept;
        void operator= (std::initializer_list<FloatVar> init_list) noexcept;
        void operator= (const std::vector<FloatVarBaseExpander>& float_vars) noexcept;
        void operator= (std::initializer_list<FloatVarBaseExpander> init_list) noexcept;
        void operator= (std::vector<FloatVar>&& float_vars) noexcept;
        void operator= (std::vector<FloatVarBaseExpander>&& float_vars) noexcept;

        void operator+= (const FloatVar& value);
        void operator+= (const FloatVarBaseExpander& value);
        void operator+= (const FloatVarObjectList& value);

        size_t size() const noexcept;
        size_t max_size() const noexcept;

        bool name_valid(const std::string& name) const noexcept;
        bool empty() const noexcept;

        void push_back(const FloatVar& floatVar);
        void erase(decltype(FloatVar::list.begin()) iter);
        void erase(const std::string& name);
        void insert(decltype(FloatVar::list.begin()) iter, const FloatVar& value);
        void clear() noexcept;
        void refresh_names() noexcept;

        FloatVar& operator [](const std::string& name);
        const FloatVar& at(const std::string& name) const;
        FloatVar& back() noexcept;
        FloatVar& front() noexcept;

        decltype(FloatVar::list.begin()) begin() noexcept;
        decltype(FloatVar::list.end()) end() noexcept;

        decltype(FloatVar::list.cbegin()) cbegin() const noexcept;
        decltype(FloatVar::list.cend()) cend() const noexcept;

        decltype(FloatVar::list.rbegin()) rbegin() noexcept;
        decltype(FloatVar::list.rend()) rend() noexcept;

        decltype(FloatVar::list.crbegin()) crbegin() const noexcept;
        decltype(FloatVar::list.crend()) crend() const noexcept;

        bool isClassAffordable();
        virtual void setType();

        size_t getIndexOfName(const std::string& name) const;
    protected:
        char mType[5];

        std::vector<std::string> mIndexMap;
    };
}

