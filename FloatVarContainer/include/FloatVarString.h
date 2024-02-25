#pragma once
#include "FloatVarBaseExpander.h"

namespace float_var_container_lib {
    class FloatVarString :
        public FloatVarBaseExpander {
    public:
        FloatVarString() = default;
        FloatVarString(const char* value);
        FloatVarString(const std::string& value);
        FloatVarString(const std::string& variableName, const std::string& value);
        FloatVarString(const FloatVar& instance);
        FloatVarString(FloatVar&& instance);

        void operator=(const FloatVar& instance);
        void operator=(const std::string& str);

        std::string getValue() const noexcept;

        bool isClassAffordable() override;

        static const char* sType;
    protected:

    };
}

