#pragma once
#include "FloatVarObjectList.h"

namespace float_var_container_lib {
    class FloatVarArray
        : public FloatVarObjectList {
    public:
        FloatVarArray();
        FloatVarArray(const FloatVarObjectList& inst);
        FloatVarArray(const FloatVarArray& inst);
        FloatVarArray(const std::vector<FloatVar>& float_vars) noexcept;
        FloatVarArray(std::initializer_list<FloatVar> init_list);
        FloatVarArray(const std::vector<FloatVarBaseExpander>& init_list);
        FloatVarArray(std::initializer_list<FloatVarBaseExpander> init_list);
        FloatVarArray(FloatVarObjectList&& inst) noexcept;
        FloatVarArray(FloatVarArray&& inst) noexcept;
        FloatVarArray(std::vector<FloatVar>&& float_vars) noexcept;
        virtual ~FloatVarArray() {}

        void operator= (const FloatVarArray& float_vars) noexcept;
        void operator= (const FloatVarObjectList& float_vars) noexcept;
        void operator= (FloatVarObjectList&& float_vars) noexcept;
        void operator= (const std::vector<FloatVar>& float_vars) noexcept;
        void operator= (std::initializer_list<FloatVar> init_list) noexcept;
        void operator= (const std::vector<FloatVarBaseExpander>& float_vars) noexcept;
        void operator= (std::initializer_list<FloatVarBaseExpander> init_list) noexcept;
        void operator= (std::vector<FloatVar>&& float_vars) noexcept;
        void operator= (std::vector<FloatVarBaseExpander>&& float_vars) noexcept;
;
        void operator+= (const FloatVarArray& value);

        FloatVar& operator[] (size_t index);
        const FloatVar& at(size_t index) const;

        void setType() override;
    };
}

