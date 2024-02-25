#include "FloatVar.h"

namespace float_var_container_lib {
    FloatVar::FloatVar() noexcept {
        memcpy(data_type, gDefaultDataType, 5);
    }

    FloatVar::FloatVar(const FloatVar& floatVar) noexcept {
        binary = floatVar.binary;
        list = floatVar.list;
        name = floatVar.name;
        memcpy(data_type, floatVar.data_type, 5);
    }

    FloatVar::FloatVar(FloatVar&& floatVar) noexcept {
        binary = std::move(floatVar.binary);
        memcpy(data_type, floatVar.data_type, 5);
        memset(floatVar.data_type, 0, 5);
        list = std::move(floatVar.list);
        name = std::move(floatVar.name);
    }

    void FloatVar::operator=(const FloatVar& floatVar) noexcept {
        binary = floatVar.binary;
        list = floatVar.list;
        name = floatVar.name;
        memcpy(data_type, floatVar.data_type, 5);
    }

    bool FloatVar::operator==(const FloatVar& floatVar) noexcept {
        // compare by value
        if (binary.size() != floatVar.binary.size())
            return false;

        if (list.size() != floatVar.list.size())
            return false;

        if (binary.size()) {
            return binary == floatVar.binary;
        }
        else {
            for (size_t i = 0; i < list.size(); i++) {
                if (list[i] != floatVar.list[i])
                    return false;
            }
            return true;
        }
    }

    bool FloatVar::operator!=(const FloatVar& floatVar) noexcept {
        // compare by value
        if (binary.size() != floatVar.binary.size())
            return true;

        if (list.size() != floatVar.list.size())
            return true;

        if (binary.size()) {
            return binary != floatVar.binary;
        } else {
            for (size_t i = 0; i < list.size(); i++) {
                if (list[i] != floatVar.list[i])
                    return true;
            }
            return false;
        }
    }
}
