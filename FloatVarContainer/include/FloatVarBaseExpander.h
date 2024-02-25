#pragma once
#include "IClassChecker.h"
#include "FloatVar.h"

namespace float_var_container_lib {
    class FloatVarBaseExpander
        : public IClassChecker {
    public:
        FloatVarBaseExpander() noexcept;
        FloatVarBaseExpander(const std::string& variableName) noexcept;
        FloatVarBaseExpander(const FloatVar& floatVar) noexcept;
        FloatVarBaseExpander(FloatVar&& floatVar) noexcept;
        FloatVarBaseExpander(const FloatVarBaseExpander& floatVar) noexcept;
        FloatVarBaseExpander(FloatVarBaseExpander&& floatVar) noexcept;

        void operator=(const FloatVar& floatVar) noexcept;
        bool operator== (const FloatVar& floatVar) noexcept;
        bool operator!= (const FloatVar& floatVar) noexcept;
        void operator=(const FloatVarBaseExpander& floatVar) noexcept;
        bool operator== (const FloatVarBaseExpander& floatVar) noexcept;
        bool operator!= (const FloatVarBaseExpander& floatVar) noexcept;
        inline virtual ~FloatVarBaseExpander() {}

        FloatVar* operator() () noexcept;

        void clear() noexcept;

        const FloatVar* getFloatVar() const noexcept;
    protected:
        FloatVar mInstance;
    };
}
