#include "PropertyList.h"

std::vector<size_t> comp_prop_kit_lib::PropertyList::getExistingTypeHash() const noexcept {
    std::vector<size_t> ret;

    for (auto& pair : mComponents) {
        ret.push_back(pair.first);
    }

    return ret;
}
