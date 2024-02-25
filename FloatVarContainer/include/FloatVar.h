#pragma once
#include <string>
#include <vector>

#include "IClassChecker.h"

namespace float_var_container_lib {
    constexpr char gDefaultDataType[] = "void";

    // base struct, what is expandable
    struct FloatVar {
        FloatVar() noexcept;
        FloatVar(const FloatVar& floatVar) noexcept;
        FloatVar(FloatVar&& floatVar) noexcept;

        void operator=(const FloatVar& floatVar) noexcept;
        bool operator== (const FloatVar& floatVar) noexcept;
        bool operator!= (const FloatVar& floatVar) noexcept;

        template<typename T>
        typename std::enable_if<std::is_pointer<T>::value, void>::type setBinaryValue(T ptr, size_t size);
        template<typename T>
        typename std::enable_if<!std::is_pointer<T>::value, void>::type setBinaryValue(T value);

        template<typename T>
        typename std::enable_if<!std::is_pointer<T>::value, T>::type convertTo() const;
        template<typename T>
        typename std::enable_if<std::is_pointer<T>::value, T>::type convertTo() const;

        std::string name;
        char data_type[5];

        std::vector<FloatVar> list;
        std::string binary;
    };

    template<typename T>
    inline typename std::enable_if<std::is_pointer<T>::value, void>::type FloatVar::setBinaryValue(T ptr, size_t size) {
        binary.resize(size);
        memcpy(const_cast<char*>(binary.data()), ptr, size);
    }

    template<typename T>
    inline typename std::enable_if<!std::is_pointer<T>::value, void>::type FloatVar::setBinaryValue(T value) {
        if (std::is_arithmetic<T>::value) {
            binary.resize(sizeof(T));
            for (uint8_t i = 0; i < binary.size(); i++)
                binary[i] = *((char*)(&value) + i);
        } else
            setBinaryValue(&value, sizeof(T));
    }

    template<typename T>
    inline typename std::enable_if<!std::is_pointer<T>::value, T>::type FloatVar::convertTo() const {
        if (binary.size() != sizeof(T))
            return T();
        return *reinterpret_cast<const T*>(binary.c_str());
    }

    template<typename T>
    typename std::enable_if<std::is_pointer<T>::value, T>::type FloatVar::convertTo() const {
        return binary.c_str();
    }
}
