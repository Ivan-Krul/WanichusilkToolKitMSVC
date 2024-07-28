#pragma once
#include <memory>

namespace comp_prop_kit_lib {
    typedef size_t ComponentTypeHash;

    class PropertyList;

    class Component {
    public:
        inline Component(ComponentTypeHash infoHash) :
            mInfoHash(infoHash) {
        }

        virtual void init() {}

        inline _NODISCARD const ComponentTypeHash getInfoHash() const noexcept {
            return mInfoHash;
        }

        virtual ~Component() = default;
    private:
        ComponentTypeHash mInfoHash;
    };
}
