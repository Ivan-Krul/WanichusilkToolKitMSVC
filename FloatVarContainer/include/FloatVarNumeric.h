#pragma once
#include "FloatVar.h"
#include "FloatVarBaseExpander.h"

#include <vector>

namespace float_var_container_lib {
    class FloatVarNumeric
        : public FloatVarBaseExpander {
        public:
            FloatVarNumeric() = default;
            FloatVarNumeric(bool value) noexcept;
            FloatVarNumeric(char value) noexcept;
            FloatVarNumeric(short value) noexcept;
            FloatVarNumeric(int value) noexcept;
            FloatVarNumeric(long long value) noexcept;
            FloatVarNumeric(unsigned char value) noexcept;
            FloatVarNumeric(unsigned short value) noexcept;
            FloatVarNumeric(unsigned int value) noexcept;
            FloatVarNumeric(unsigned long long value) noexcept;
            FloatVarNumeric(float value) noexcept;
            FloatVarNumeric(double value) noexcept;
            FloatVarNumeric(const std::string& variableName, bool value) noexcept;
            FloatVarNumeric(const std::string& variableName, char value) noexcept;
            FloatVarNumeric(const std::string& variableName, short value) noexcept;
            FloatVarNumeric(const std::string& variableName, int value) noexcept;
            FloatVarNumeric(const std::string& variableName, long long value) noexcept;
            FloatVarNumeric(const std::string& variableName, unsigned char value) noexcept;
            FloatVarNumeric(const std::string& variableName, unsigned short value) noexcept;
            FloatVarNumeric(const std::string& variableName, unsigned int value) noexcept;
            FloatVarNumeric(const std::string& variableName, unsigned long long value) noexcept;
            FloatVarNumeric(const std::string& variableName, float value) noexcept;
            FloatVarNumeric(const std::string& variableName, double value) noexcept;
            FloatVarNumeric(const FloatVar& instance);
            FloatVarNumeric(FloatVar&& instance);
            virtual ~FloatVarNumeric() {}

            void operator=(bool value) noexcept;
            void operator=(char value) noexcept;
            void operator=(short value) noexcept;
            void operator=(int value) noexcept;
            void operator=(long long value) noexcept;
            void operator=(unsigned char value) noexcept;
            void operator=(unsigned short value) noexcept;
            void operator=(unsigned int value) noexcept;
            void operator=(unsigned long long value) noexcept;
            void operator=(float value) noexcept;
            void operator=(double value) noexcept;

            template <typename T>
            inline typename std::enable_if<
                std::is_arithmetic<T>::value, T>::type
            getValue() const noexcept;

            bool isFloat() const noexcept;
            bool isClassAffordable() override;

            void changeDataType(size_t indexOfType) noexcept;

            static const std::vector<const char*> sTypes;
        protected:
            template<typename T>
            void assignBinary(size_t typeIndex, T value) noexcept;

            bool mIsFloat = false;
    };

    template<typename T>
    inline typename std::enable_if<
        std::is_arithmetic<T>::value, T>::type
    FloatVarNumeric::getValue() const noexcept {
        return mInstance.convertTo<T>();
    }

    template<typename T>
    inline void FloatVarNumeric::assignBinary(size_t typeIndex, T value) noexcept {
        if (*(const int*)mInstance.data_type != *(const int*)sTypes[typeIndex]) {
            mInstance.binary.resize(sizeof(T));
            memcpy(mInstance.data_type, sTypes[typeIndex], 4);
        }
        mInstance.setBinaryValue(value);
    }
}
