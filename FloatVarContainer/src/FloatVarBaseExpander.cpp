#include "FloatVarBaseExpander.h"

namespace float_var_container_lib {
    FloatVarBaseExpander::FloatVarBaseExpander() noexcept {
        memcpy(mInstance.data_type, gDefaultDataType, 5);
    }

    FloatVarBaseExpander::FloatVarBaseExpander(const std::string& variableName) noexcept {
        mInstance.name = variableName;
        memcpy(mInstance.data_type, gDefaultDataType, 5);
    }

    FloatVarBaseExpander::FloatVarBaseExpander(const FloatVar& floatVar) noexcept {
        mInstance = floatVar;
        memcpy(mInstance.data_type, gDefaultDataType, 5);
    }

    FloatVarBaseExpander::FloatVarBaseExpander(FloatVar&& floatVar) noexcept {
        mInstance = floatVar;
        floatVar.binary.clear();
        floatVar.name.clear();
        floatVar.list.clear();
        memcpy(floatVar.data_type, gDefaultDataType, 5);
    }

    FloatVarBaseExpander::FloatVarBaseExpander(const FloatVarBaseExpander& floatVar) noexcept {
        mInstance = floatVar.mInstance;
    }

    FloatVarBaseExpander::FloatVarBaseExpander(FloatVarBaseExpander&& floatVar) noexcept {
        mInstance = floatVar.mInstance;
        floatVar.clear();
    }

    FloatVar* FloatVarBaseExpander::operator()() noexcept {
        return &mInstance;
    }

    void FloatVarBaseExpander::clear() noexcept {
        mInstance.binary.clear();
        mInstance.name.clear();
        mInstance.list.clear();
        memcpy(mInstance.data_type, gDefaultDataType, 5);
    }

    void FloatVarBaseExpander::operator=(const FloatVar& floatVar) noexcept {
        mInstance = floatVar;
    }

    bool FloatVarBaseExpander::operator==(const FloatVar& floatVar) noexcept {
        return mInstance == floatVar;
    }

    bool FloatVarBaseExpander::operator!=(const FloatVar& floatVar) noexcept {
        return mInstance != floatVar;
    }

    void FloatVarBaseExpander::operator=(const FloatVarBaseExpander& floatVar) noexcept {
        mInstance = floatVar.mInstance;
    }

    bool FloatVarBaseExpander::operator==(const FloatVarBaseExpander& floatVar) noexcept {
        return mInstance == floatVar.mInstance;
    }

    bool FloatVarBaseExpander::operator!=(const FloatVarBaseExpander& floatVar) noexcept {
        return mInstance != floatVar.mInstance;
    }

    const FloatVar* FloatVarBaseExpander::getFloatVar() const noexcept {
        return &mInstance;
    }
}