#pragma once
#include <memory>

namespace comp_prop_kit_lib {
    typedef size_t ComponentID;
    typedef size_t ComponentTypeHash;

    class PropertyList;

    class Component {
    public:
        inline Component(PropertyList& parent, ComponentID id, ComponentTypeHash infoHash)
            : mId(id), pParent(&parent), mInfoHash(infoHash) {
        }

        virtual void init() {}

        inline _NODISCARD const ComponentID getId() const noexcept {
            return mId;
        }

        inline _NODISCARD const PropertyList* getParent() const noexcept {
            return pParent;
        }

        inline _NODISCARD const ComponentTypeHash getInfoHash() const noexcept {
            return mInfoHash;
        }

        virtual ~Component() = default;
    private:
        ComponentID mId;
        ComponentTypeHash mInfoHash;

        PropertyList* pParent;
    };
}
