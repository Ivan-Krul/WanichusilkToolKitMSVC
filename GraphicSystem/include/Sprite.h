#pragma once
#include <memory>

#include "raylib.h"

#include "GraphicElement.h"

namespace graphic_sys_lib {
    class Sprite :
        public GraphicElement {
    public:
        inline Sprite()
            : GraphicElement(), mTint(RAYWHITE) {
            mTexture = { 0 };
        }

        inline Sprite(const Texture2D& mTexture, int mPosX, int mPosY, const Color& mTint)
             : GraphicElement(), mTexture(mTexture), mTint(mTint) {
            mProperties.getComponent<PosComponent>().setPos(mPosX, mPosY);
        }

        inline Sprite(const Texture2D& mTexture, int mPosX, int mPosY)
            : GraphicElement(), mTexture(mTexture), mTint(RAYWHITE) {
            mProperties.getComponent<PosComponent>().setPos(mPosX, mPosY);
        }

        inline Sprite(const Texture2D& mTexture, const Color& mTint)
            : GraphicElement(), mTexture(mTexture),  mTint(mTint) {
        }

        inline Sprite(const Texture2D& mTexture)
            : GraphicElement(), mTexture(mTexture),  mTint(RAYWHITE) {
        }

        inline void render() const override {
            auto comp = mProperties.getComponent<PosComponent>();
            DrawTexture(mTexture, comp.getPosX<int>(), comp.getPosY<int>(), mTint);
        }

        void parseFile(const std::string& pathToCMPT) override;
        _NODISCARD bool isParsableFile(const std::string& pathToCMPT) override;
        void parseFloatVar(const hardware_envi_lib::float_var floatVar) override;
        _NODISCARD bool isParsableFloatVar(const hardware_envi_lib::float_var floatVar) override;

        inline _NODISCARD Color getTint() const {
            return mTint;
        }

        inline _NODISCARD Texture2D getTexuture() const {
            return mTexture;
        }

        inline _NODISCARD Texture2D& getTexuture() {
            return mTexture;
        }

        inline void setTint(const Color tint) {
            mTint = tint;
        }

        ~Sprite() {
            UnloadTexture(mTexture);
        }

    private:
        Texture2D mTexture;
        Color mTint;

    };
}
