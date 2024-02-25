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
