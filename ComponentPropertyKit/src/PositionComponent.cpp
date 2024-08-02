#include "ComponentPropertyKit.h"

namespace comp_prop_kit_lib {
    void PositionComponent::setDir(float x, float y) noexcept {
        mDirX = x;
        mDirY = y;
    }

    void PositionComponent::setDir(int x, int y) noexcept {
        mDirX = static_cast<float>(x);
        mDirY = static_cast<float>(y);
    }

    void PositionComponent::shift() noexcept {
        mPosX += mDirX;
        mPosY += mDirY;
    }
}

template<>
comp_prop_kit_lib::PositionComponent& comp_prop_kit_lib::PropertyList::pushComponent<comp_prop_kit_lib::PositionComponent>();
template<>
comp_prop_kit_lib::PositionComponent& comp_prop_kit_lib::PropertyList::getComponent<comp_prop_kit_lib::PositionComponent>();
template<>
comp_prop_kit_lib::PositionComponent comp_prop_kit_lib::PropertyList::getComponent<comp_prop_kit_lib::PositionComponent>() const;
template<>
void comp_prop_kit_lib::PropertyList::popComponent<comp_prop_kit_lib::PositionComponent>();
