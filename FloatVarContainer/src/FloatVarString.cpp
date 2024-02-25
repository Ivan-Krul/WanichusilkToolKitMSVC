#include "FloatVarString.h"

namespace float_var_container_lib {
    FloatVarString::FloatVarString(const char* value) {
        mInstance.binary = value;
        memcpy(mInstance.data_type, sType, 5);
    }

    FloatVarString::FloatVarString(const std::string& value) {
        mInstance.binary = value;
        memcpy(mInstance.data_type, sType, 5);
    }

    FloatVarString::FloatVarString(const std::string& variableName, const std::string& value) {
        mInstance.binary = value;
        mInstance.name = variableName;
        memcpy(mInstance.data_type, sType, 5);
    }

    FloatVarString::FloatVarString(const FloatVar& instance) {
        mInstance = instance;
    }

    FloatVarString::FloatVarString(FloatVar&& instance) {
        mInstance = instance;
    }

    void FloatVarString::operator=(const std::string& str) {
        if(!isClassAffordable())
            memcpy(mInstance.data_type, sType, 5);
        mInstance.binary = str;
    }

    std::string FloatVarString::getValue() const noexcept {
        return mInstance.binary;
    }

    bool FloatVarString::isClassAffordable() {
        return *(const int*)sType == *(const int*)mInstance.data_type;
    }

    void FloatVarString::operator=(const FloatVar& instance) {
        mInstance = instance;
    }

    const char* FloatVarString::sType = "strg";
}
