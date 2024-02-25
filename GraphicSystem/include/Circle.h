#pragma once
#include "GraphicElement.h"

namespace graphic_sys_lib {
    class Circle :
        public GraphicElement {
    public:
        inline Circle() : GraphicElement() {
            mRadius = 1.0f;
            mColor = RAYWHITE;
        }
        inline Circle(int posX, int posY) : GraphicElement() {
            mRadius = 1.0f;
            mColor = RAYWHITE;
        }
        inline Circle(int posX, int posY, float radius) : GraphicElement() {
            mRadius = radius;
            mProperties.getComponent<PosComponent>(0).setPos(posX, posY);
            mColor = RAYWHITE;
        }
        inline Circle(int posX, int posY, float radius, Color color) : GraphicElement() {
            mProperties.getComponent<PosComponent>(0).setPos(posX, posY);
            mRadius = radius;
            mColor = color;
        }

        inline void render() const override {
            auto comp = mProperties.getComponent<PosComponent>(0);
            DrawCircle(comp.getPosX<int>(), comp.getPosY<int>(), mRadius, mColor);
        }

        void parseFile(const std::string& pathToCMPT) override;
        _NODISCARD bool isParsableFile(const std::string& pathToCMPT) override;
        void parseFloatVar(const hardware_envi_lib::float_var floatVar) override;
        _NODISCARD bool isParsableFloatVar(const hardware_envi_lib::float_var floatVar) override;

        inline void setRadius(float radius) noexcept {
            mRadius = radius;
        }

        inline void setColor(Color color) noexcept {
            mColor = color;
        }

        inline _NODISCARD float getRadius() const noexcept {
            return mRadius;
        }

        inline _NODISCARD Color getColor() const noexcept {
            return mColor;
        }
    private:
        Color mColor;
        float mRadius;

    };
}
