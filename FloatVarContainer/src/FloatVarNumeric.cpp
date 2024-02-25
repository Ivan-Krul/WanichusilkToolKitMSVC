#include "FloatVarNumeric.h"

namespace float_var_container_lib {
    FloatVarNumeric::FloatVarNumeric(bool value) noexcept {
        mInstance.setBinaryValue(value);
        memcpy(mInstance.data_type, sTypes[0], 5);
    }

    FloatVarNumeric::FloatVarNumeric(char value) noexcept {
        mInstance.setBinaryValue(value);
        memcpy(mInstance.data_type, sTypes[1], 5);
    }

    FloatVarNumeric::FloatVarNumeric(short value) noexcept {
        mInstance.setBinaryValue(value);
        memcpy(mInstance.data_type, sTypes[2], 5);
    }

    FloatVarNumeric::FloatVarNumeric(int value) noexcept {
        mInstance.setBinaryValue(value);
        memcpy(mInstance.data_type, sTypes[3], 5);
    }

    FloatVarNumeric::FloatVarNumeric(long long value) noexcept {
        mInstance.setBinaryValue(value);
        memcpy(mInstance.data_type, sTypes[4], 5);
    }

    FloatVarNumeric::FloatVarNumeric(unsigned char value) noexcept {
        mInstance.setBinaryValue(value);
        memcpy(mInstance.data_type, sTypes[5], 5);
    }

    FloatVarNumeric::FloatVarNumeric(unsigned short value) noexcept {
        mInstance.setBinaryValue(value);
        memcpy(mInstance.data_type, sTypes[6], 5);
    }

    FloatVarNumeric::FloatVarNumeric(unsigned int value) noexcept {
        mInstance.setBinaryValue(value);
        memcpy(mInstance.data_type, sTypes[7], 5);
    }

    FloatVarNumeric::FloatVarNumeric(unsigned long long value) noexcept {
        mInstance.setBinaryValue(value);
        memcpy(mInstance.data_type, sTypes[8], 5);
    }

    FloatVarNumeric::FloatVarNumeric(float value) noexcept {
        mInstance.setBinaryValue(value);
        memcpy(mInstance.data_type, sTypes[9], 5);
        mIsFloat = true;
    }

    FloatVarNumeric::FloatVarNumeric(double value) noexcept {
        mInstance.setBinaryValue(value);
        memcpy(mInstance.data_type, sTypes[10], 5);
        mIsFloat = true;
    }
    FloatVarNumeric::FloatVarNumeric(const std::string& variableName, bool value) noexcept
        : FloatVarNumeric::FloatVarNumeric(value) {
        mInstance.name = variableName;
    }

    FloatVarNumeric::FloatVarNumeric(const std::string& variableName, char value) noexcept
        : FloatVarNumeric::FloatVarNumeric(value) {
        mInstance.name = variableName;
    }

    FloatVarNumeric::FloatVarNumeric(const std::string& variableName, short value) noexcept
        : FloatVarNumeric::FloatVarNumeric(value) {
        mInstance.name = variableName;
    }

    FloatVarNumeric::FloatVarNumeric(const std::string& variableName, int value) noexcept
        : FloatVarNumeric::FloatVarNumeric(value) {
        mInstance.name = variableName;
    }

    FloatVarNumeric::FloatVarNumeric(const std::string& variableName, long long value) noexcept
        : FloatVarNumeric::FloatVarNumeric(value) {
        mInstance.name = variableName;
    }

    FloatVarNumeric::FloatVarNumeric(const std::string& variableName, unsigned char value) noexcept
        : FloatVarNumeric::FloatVarNumeric(value) {
        mInstance.name = variableName;
    }

    FloatVarNumeric::FloatVarNumeric(const std::string& variableName, unsigned short value) noexcept
        : FloatVarNumeric::FloatVarNumeric(value) {
        mInstance.name = variableName;
    }

    FloatVarNumeric::FloatVarNumeric(const std::string& variableName, unsigned int value) noexcept
        : FloatVarNumeric::FloatVarNumeric(value) {
        mInstance.name = variableName;
    }

    FloatVarNumeric::FloatVarNumeric(const std::string& variableName, unsigned long long value) noexcept
        : FloatVarNumeric::FloatVarNumeric(value) {
        mInstance.name = variableName;
    }

    FloatVarNumeric::FloatVarNumeric(const std::string& variableName, float value) noexcept
        : FloatVarNumeric::FloatVarNumeric(value) {
        mInstance.name = variableName;
    }

    FloatVarNumeric::FloatVarNumeric(const std::string& variableName, double value) noexcept
        : FloatVarNumeric::FloatVarNumeric(value) {
        mInstance.name = variableName;
    }

    FloatVarNumeric::FloatVarNumeric(const FloatVar& instance) {
        mInstance = instance;
    }

    FloatVarNumeric::FloatVarNumeric(FloatVar&& instance) {
        mInstance = instance;
    }

    void FloatVarNumeric::operator=(bool value) noexcept {
        changeDataType(0);
        assignBinary(0, value);
    }

    void FloatVarNumeric::operator=(char value) noexcept {
        changeDataType(1); 
        assignBinary(1, value);
    }

    void FloatVarNumeric::operator=(short value) noexcept {
        changeDataType(2); 
        assignBinary(2, value);
    }

    void FloatVarNumeric::operator=(int value) noexcept {
        changeDataType(3); 
        assignBinary(3, value);
    }

    void FloatVarNumeric::operator=(long long value) noexcept {
        changeDataType(4);
        assignBinary(4, value);
    }

    void FloatVarNumeric::operator=(unsigned char value) noexcept {
        changeDataType(5);
        assignBinary(5, value);
    }

    void FloatVarNumeric::operator=(unsigned short value) noexcept {
        changeDataType(6);
        assignBinary(6, value);
    }

    void FloatVarNumeric::operator=(unsigned int value) noexcept {
        changeDataType(7);
        assignBinary(7, value);
    }

    void FloatVarNumeric::operator=(unsigned long long value) noexcept {
        changeDataType(8);
        assignBinary(8, value);
    }

    void FloatVarNumeric::operator=(float value) noexcept {
        changeDataType(9);
        assignBinary(9, value);
    }

    void FloatVarNumeric::operator=(double value) noexcept {
        changeDataType(10);
        assignBinary(10, value);
    }

    bool FloatVarNumeric::isFloat() const noexcept {
        return mIsFloat;
    }

    bool FloatVarNumeric::isClassAffordable() {
        for(const auto &el : sTypes) {
            if (*(const int*)el == *(const int*)mInstance.data_type) // more efficient to checking numbers, but not strings
                return true;
        }
        return false;
    }

    void FloatVarNumeric::changeDataType(size_t indexOfType) noexcept {
        if (*(const int*)mInstance.data_type == *(const int*)sTypes[indexOfType])
            memcpy(mInstance.data_type, sTypes[indexOfType], 5);
    }

    const std::vector<const char*> FloatVarNumeric::sTypes = {
        "nbol","nchr","nshr","nint","nlg8","nuch","nush","nuin","nulg","nflt","ndbl"
    };
}
