#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <exception>

#include "FloatVar.h"

#include "GraphicElement.h"

namespace graphic_sys_lib {
    class GraphicElementFactory {
    public:
        template<typename... Types>
        struct TypeList {};

        template<typename... Types>
        std::vector<std::unique_ptr<GraphicElement>> parseFloatVarArr(const hardware_envi_lib::float_var& floatVar, TypeList<Types...>&);

    protected:
        template<typename T, typename... Rest>
        void parseType(const TypeList<T, Rest...>&) {
            static_assert(std::is_base_of<T, GraphicElement>::value, "assert: template in parseType isn't based on GraphicElement");

            if (mBufferisedFloatVar[0]() == typeid(T).name()) {
                T* elem = new T();
                std::unique_ptr<GraphicElement> uGrEl{elem};

                if (!uGrEl->isParsableFloatVar(mBufferisedFloatVar[1])) {
                    std::cerr << "bad typeid: bad recognision\n";
                    throw std::bad_typeid("bad recognision");
                }

                uGrEl->parseFloatVar(mBufferisedFloatVar[1]);

                mElements.push_back(std::move(uGrEl));
                return;
            }

            parseType(TypeList<Rest...>());
        }

        void parseType(const TypeList<>&) {
            std::clog << "log: element wasn't recognised\n";
        }

            std::vector<std::unique_ptr<GraphicElement>> mElements;
        hardware_envi_lib::float_var mBufferisedFloatVar;
    };

    template<typename ...Types>
    inline std::vector<std::unique_ptr<GraphicElement>> GraphicElementFactory::parseFloatVarArr(const hardware_envi_lib::float_var& floatVar, TypeList<Types...>&) {
        if (floatVar.size() == 0 || floatVar.data_type != hardware_envi_lib::DataType::arr__) {
            std::cerr << "runtime: float_var isn't array, or it's empty\n";
            throw std::runtime_error("float_var isn't array, or it's empty");
        }

        for (size_t i = 0; i < floatVar.size(); i++) {
            mBufferisedFloatVar = floatVar[i];
            if (mBufferisedFloatVar.data_type != hardware_envi_lib::DataType::arr__) {
                std::clog << "log: element with index " << i << " isn't array\n";
                continue;
            }
            if (mBufferisedFloatVar.size() != 2) {
                std::clog << "log: element with index " << i << " hasn't size equat to 2\n";
                continue;
            }
            if (mBufferisedFloatVar[0].data_type != hardware_envi_lib::DataType::str__) {
                std::clog << "log: element with index " << i << " hasn't str_ type at 0 index\n";
                continue;
            }
            if (mBufferisedFloatVar[1].data_type != hardware_envi_lib::DataType::arr__) {
                std::clog << "log: element with index " << i << " hasn't arr_ type at 1 index\n";
                continue;
            }
            parseType(TypeList<Types...>());
        }

        return mElements;
    }
}
