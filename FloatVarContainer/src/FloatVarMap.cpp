#include "FloatVarMap.h"

namespace float_var_container_lib {
	FloatVarMap::FloatVarMap() {
		mInstance.list.resize(2);
		memcpy(mInstance.list[0].data_type, "mpki", 5);
		memcpy(mInstance.list[1].data_type, "mpvl", 5);
		setType();
	}

	FloatVarMap::FloatVarMap(const FloatVarMap& inst) {
		mIndexMap = inst.mIndexMap;
		mInstance = inst.mInstance;
		setType();
	}

	FloatVarMap::FloatVarMap(FloatVarMap&& inst) noexcept {
		mInstance = inst.mInstance;
		mIndexMap = inst.mIndexMap;
		setType();
		inst.clear();
	}

	void FloatVarMap::operator=(const FloatVarMap& float_vars) noexcept {
		mInstance = float_vars.mInstance;
		mIndexMap = float_vars.mIndexMap;
		memcpy(mInstance.list[0].data_type, "mpki", 5);
		memcpy(mInstance.list[1].data_type, "mpvl", 5);
	}

	FloatVar& FloatVarMap::getKeys() noexcept {
		return mInstance.list[0];
	}

	FloatVar& FloatVarMap::getValues() noexcept {
		return mInstance.list[1];
	}

	void FloatVarMap::insert(const FloatVar& key, const FloatVar& value) {
		mInstance.list[0].list.push_back(key);
		mInstance.list[1].list.push_back(value);
	}

	decltype(FloatVar::list.begin()) FloatVarMap::getValue(const FloatVar& key) {
		auto iter = std::find(mInstance.list[0].list.begin(), mInstance.list[0].list.end(), key);

		size_t index = std::distance(mInstance.list[0].list.begin(), iter);

		if (index < mInstance.list[1].list.size())
			return mInstance.list[1].list.begin() + index;

		return mInstance.list[1].list.end();
	}

	void FloatVarMap::setType() {
		memcpy(mInstance.data_type, "map_", 5);
	}
}
