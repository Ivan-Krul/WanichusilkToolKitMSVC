#include "FloatVarObjectList.h"

namespace float_var_container_lib {
    FloatVarObjectList::FloatVarObjectList() {
        setType();
        memcpy(mInstance.data_type, mType, 5);
    }

    FloatVarObjectList::FloatVarObjectList(const FloatVarObjectList& inst) {
        mInstance = inst.mInstance;
        mIndexMap = inst.mIndexMap;
        setType();
        memcpy(mInstance.data_type, mType, 5);
    }

    FloatVarObjectList::FloatVarObjectList(FloatVarObjectList&& inst) noexcept {
        mInstance = inst.mInstance;
        mIndexMap = inst.mIndexMap;
        inst.clear();
        setType();
        memcpy(mInstance.data_type, mType, 5);
    }

    FloatVarObjectList::FloatVarObjectList(std::initializer_list<FloatVar> init_list) {
        for (auto& init : init_list) {
            mInstance.list.push_back(init);
        }
        refresh_names();
        setType();
        memcpy(mInstance.data_type, mType, 5);
    }

    FloatVarObjectList::FloatVarObjectList(std::initializer_list<FloatVarBaseExpander> init_list) {
        for (auto& init : init_list) {
            mInstance.list.push_back(*init.getFloatVar());
        }
        refresh_names();
        setType();
        memcpy(mInstance.data_type, mType, 5);
    }

    FloatVarObjectList::FloatVarObjectList(const std::vector<FloatVarBaseExpander>& init_list) {
        for (auto& init : init_list) {
            mInstance.list.push_back(*init.getFloatVar());
        }
        refresh_names();
        setType();
        memcpy(mInstance.data_type, mType, 5);
    }

    FloatVarObjectList::FloatVarObjectList(const std::vector<FloatVar>& float_vars) noexcept {
        mInstance.list = float_vars;
        refresh_names();
        setType();
        memcpy(mInstance.data_type, mType, 5);
    }

    FloatVarObjectList::FloatVarObjectList(std::vector<FloatVar>&& float_vars) noexcept {
        mInstance.list = float_vars;
        float_vars.clear();
        refresh_names();
        setType();
        memcpy(mInstance.data_type, mType, 5);
    }

    void FloatVarObjectList::operator=(const FloatVarObjectList& float_vars) noexcept {
        mInstance = float_vars.mInstance;
        mIndexMap = float_vars.mIndexMap;
    }

    void FloatVarObjectList::operator=(FloatVarObjectList&& float_vars) noexcept {
        mInstance = float_vars.mInstance;
        mIndexMap = float_vars.mIndexMap;
        float_vars.clear();
    }

    void FloatVarObjectList::operator=(const std::vector<FloatVar>& float_vars) noexcept {
        mInstance.list = float_vars;
        refresh_names();
    }

    void FloatVarObjectList::operator=(std::initializer_list<FloatVar> init_list) noexcept {
        for (auto& init : init_list) {
            mInstance.list.push_back(init);
        }
        refresh_names();
    }

    void FloatVarObjectList::operator=(const std::vector<FloatVarBaseExpander>& float_vars) noexcept {
        for (auto& init : float_vars) {
            mInstance.list.push_back(*init.getFloatVar());
        }
        refresh_names();
    }

    void FloatVarObjectList::operator=(std::initializer_list<FloatVarBaseExpander> init_list) noexcept {
        for (auto& init : init_list) {
            mInstance.list.push_back(*init.getFloatVar());
        }
        refresh_names();
    }

    void FloatVarObjectList::operator=(std::vector<FloatVar>&& float_vars) noexcept {
        mInstance.list = float_vars;
        float_vars.clear();
        refresh_names();
    }

    void FloatVarObjectList::operator+=(const FloatVar& value) {
        push_back(value);
    }

    void FloatVarObjectList::operator+=(const FloatVarBaseExpander& value) {
        push_back(*value.getFloatVar());
    }

    void FloatVarObjectList::operator+=(const FloatVarObjectList& value) {
        mInstance.list.insert(mInstance.list.cend(), value.cbegin(), value.cend());
        refresh_names();
    }

    size_t FloatVarObjectList::size() const noexcept {
        return mInstance.list.size();
    }

    size_t FloatVarObjectList::max_size() const noexcept {
        return mInstance.list.max_size();
    }

    bool FloatVarObjectList::name_valid(const std::string& name) const noexcept {
        return std::find(mIndexMap.begin(), mIndexMap.end() ,name) != mIndexMap.end();
    }

    bool FloatVarObjectList::empty() const noexcept {
        return mInstance.list.empty();
    }

    void FloatVarObjectList::push_back(const FloatVar& floatVar) {
        mInstance.list.push_back(floatVar);
        mIndexMap.push_back(floatVar.name);
    }

    void FloatVarObjectList::erase(decltype(FloatVar::list.begin()) iter) {
        auto ptrToBegin = &*mInstance.list.begin();
        auto ptrToTarget = &*iter;
        size_t index = std::distance(mInstance.list.begin(), iter);

        mIndexMap.erase(mIndexMap.begin() + index);
        mInstance.list.erase(iter);
    }

    void FloatVarObjectList::erase(const std::string& name) {
        size_t index = getIndexOfName(name);

        if (index >= mIndexMap.size())
            return;

        mIndexMap.erase(mIndexMap.begin() + index);
        mInstance.list.erase(mInstance.list.begin() + index);
    }

    void FloatVarObjectList::insert(decltype(FloatVar::list.begin()) iter, const FloatVar& value) {
        mInstance.list.insert(iter, value);  
        refresh_names();
    }

    void FloatVarObjectList::clear() noexcept {
        mInstance.list.clear();
        mIndexMap.clear();
    }

    void FloatVarObjectList::refresh_names() noexcept {
        mIndexMap.clear();
        mIndexMap.resize(mInstance.list.size());
        for (size_t i = 0; i < mInstance.list.size(); i++) {
            mIndexMap[i] = mInstance.list[i].name;
        }
    }

    FloatVar& FloatVarObjectList::operator[](const std::string& name) {
        return mInstance.list[getIndexOfName(name)];
    }

    const FloatVar& FloatVarObjectList::at(const std::string& name) const {
        return mInstance.list.at(getIndexOfName(name));
    }

    FloatVar& FloatVarObjectList::back() noexcept {
        return mInstance.list.back();
    }

    FloatVar& FloatVarObjectList::front() noexcept {
        return mInstance.list.front();
    }

    decltype(FloatVar::list.begin()) FloatVarObjectList::begin() noexcept {
        return mInstance.list.begin();
    }

    decltype(FloatVar::list.end()) FloatVarObjectList::end() noexcept {
        return mInstance.list.end();
    }

    decltype(FloatVar::list.cbegin()) FloatVarObjectList::cbegin() const noexcept {
        return mInstance.list.cbegin();
    }

    decltype(FloatVar::list.cend()) FloatVarObjectList::cend() const noexcept {
        return mInstance.list.cend();
    }

    decltype(FloatVar::list.rbegin()) FloatVarObjectList::rbegin() noexcept {
        return mInstance.list.rbegin();
    }

    decltype(FloatVar::list.rend()) FloatVarObjectList::rend() noexcept {
        return mInstance.list.rend();
    }

    decltype(FloatVar::list.crbegin()) FloatVarObjectList::crbegin() const noexcept {
        return mInstance.list.crbegin();
    }

    decltype(FloatVar::list.crend()) FloatVarObjectList::crend() const noexcept {
        return mInstance.list.crend();
    }

    bool FloatVarObjectList::isClassAffordable() {
        return mInstance.binary.empty() && *(const int*)mInstance.data_type == *(const int*)mType;
    }

    void FloatVarObjectList::setType() {
        memcpy(mType, "objt", 5);
    }

    size_t FloatVarObjectList::getIndexOfName(const std::string& name) const {
        return std::distance(
            mInstance.list.begin(),
            std::find_if(mInstance.list.begin(),
                mInstance.list.end(), 
                [&name](const FloatVar& fv) {
                    return fv.name == name; 
                }));
    }
}
