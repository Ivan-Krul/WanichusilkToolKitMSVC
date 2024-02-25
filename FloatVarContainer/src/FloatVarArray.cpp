#include "FloatVarArray.h"

namespace float_var_container_lib {
	FloatVar& FloatVarArray::operator[](size_t index) {
		return mInstance.list[index];
	}

	const FloatVar& FloatVarArray::at(size_t index) const {
		return mInstance.list.at(index);
	}

	void FloatVarArray::setType() {
		memcpy(mType, "aray", 5);
	}

	FloatVarArray::FloatVarArray() {
		setType();
		memcpy(mInstance.data_type, mType, 5);
	}

	FloatVarArray::FloatVarArray(const FloatVarObjectList& inst)
		: FloatVarObjectList(inst) {
	}

	FloatVarArray::FloatVarArray(const FloatVarArray& inst) {
		mInstance = inst.mInstance;
		mIndexMap = inst.mIndexMap;
		memcpy(mInstance.data_type, mType, 5);
		setType();
	}

	FloatVarArray::FloatVarArray(const std::vector<FloatVar>& float_vars) noexcept
		: FloatVarObjectList(float_vars) {
	}

	FloatVarArray::FloatVarArray(std::initializer_list<FloatVar> init_list)
		: FloatVarObjectList(init_list) {
	}

	FloatVarArray::FloatVarArray(FloatVarObjectList&& inst) noexcept
		: FloatVarObjectList(inst) {
	}

	FloatVarArray::FloatVarArray(FloatVarArray&& inst) noexcept
		: FloatVarObjectList(inst) {
	}

	FloatVarArray::FloatVarArray(std::vector<FloatVar>&& float_vars) noexcept
		: FloatVarObjectList(float_vars) {
	}

	FloatVarArray::FloatVarArray(const std::vector<FloatVarBaseExpander>& init_list)
		: FloatVarObjectList(init_list) {
	}

	void FloatVarArray::operator=(const FloatVarArray& float_vars) noexcept {
		mInstance = float_vars.mInstance;
		mIndexMap = float_vars.mIndexMap;
		memcpy(mInstance.data_type, mType, 5);
	}

	void FloatVarArray::operator=(const FloatVarObjectList& float_vars) noexcept {
		clear();
		mIndexMap.reserve(float_vars.size());
		mInstance.list.reserve(float_vars.size());
		for (auto it = float_vars.cbegin(); it != float_vars.cend(); it++) {
			mInstance.list.push_back(*it);
			mIndexMap.push_back(it->name);
		}
	}

	void FloatVarArray::operator=(FloatVarObjectList&& float_vars) noexcept {
		operator=(float_vars);
		float_vars.clear();
	}

	void FloatVarArray::operator=(const std::vector<FloatVar>& float_vars) noexcept {
		clear();
		mIndexMap.reserve(float_vars.size());
		mInstance.list.reserve(float_vars.size());
		for (auto& el : float_vars) {
			mInstance.list.push_back(el);
			mIndexMap.push_back(el.name);
		}
	}

	void FloatVarArray::operator=(std::initializer_list<FloatVar> init_list) noexcept {
		clear();
		mIndexMap.reserve(init_list.size());
		mInstance.list.reserve(init_list.size());
		for (auto& el : init_list) {
			mInstance.list.push_back(el);
			mIndexMap.push_back(el.name);
		}
	}

	void FloatVarArray::operator=(std::vector<FloatVar>&& float_vars) noexcept {
		operator=(float_vars);
		float_vars.clear();
	}

	void FloatVarArray::operator+=(const FloatVarArray& value) {
		for (auto it = value.cbegin(); it != value.cend(); it++) {
			mInstance.list.push_back(*it);
			mIndexMap.push_back(it->name);
		}
	}
}
