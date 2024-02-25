#pragma once
#include "FloatVarObjectList.h"

namespace float_var_container_lib {
	class FloatVarMap
		: public FloatVarObjectList {
    public:
        FloatVarMap();
        FloatVarMap(const FloatVarMap& inst);
        FloatVarMap(FloatVarMap&& inst) noexcept;
        virtual ~FloatVarMap() {}

        void operator= (const FloatVarMap& float_vars) noexcept;

        FloatVar& getKeys() noexcept;
        FloatVar& getValues() noexcept;
        void insert(const FloatVar& key, const FloatVar& value);
        decltype(FloatVar::list.begin()) getValue(const FloatVar& key);

        void setType() override;
	};
}

