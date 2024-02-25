#pragma once
#include "Component.h"

namespace comp_prop_kit_lib {
    class PositionComponent : public Component {
    public:
        inline PositionComponent(PropertyList& parent, ComponentID id, ComponentTypeHash infoHash)
            : Component(parent, id, infoHash) {

       }

        void init() override {
            mPosX = 0;
            mPosY = 0;
        }

        void setPosX(float x) noexcept {
            mPosX = x;
        }

        void setPosY(float y) noexcept {
            mPosY = y;
        }

        void setDirX(float x) noexcept {
            mDirX = x;
        }

        void setDirY(float y) noexcept {
            mDirY = y;
        }

        void setPos(float x, float y) noexcept {
            mPosX = x;
            mPosY = y;
        }

        void setPos(int x, int y) noexcept {
            mPosX = static_cast<float>(x);
            mPosY = static_cast<float>(y);
        }

        void setDir(float x, float y) noexcept;

        void setDir(int x, int y) noexcept;

        template<typename T>
        T getPosX() const noexcept {
            return static_cast<T>(mPosX);
        }

        template<typename T>
        T getPosY() const noexcept {
            return static_cast<T>(mPosY);
        }

        template<typename T>
        T getDirX() const noexcept {
            return static_cast<T>(mDirX);
        }

        template<typename T>
        T getDirY() const noexcept {
            return static_cast<T>(mDirY);
        }

        void shift() noexcept;

        template<typename T>
        void shift(T X, T Y) noexcept {
            mPosX += static_cast<float>(X);
            mPosY += static_cast<float>(Y);
        }
    private:
        float mDirX = 0.0f;
        float mDirY = 0.0f;

        float mPosX = 0.0f;
        float mPosY = 0.0f;
    };
}

typedef comp_prop_kit_lib::PositionComponent PosComponent;
