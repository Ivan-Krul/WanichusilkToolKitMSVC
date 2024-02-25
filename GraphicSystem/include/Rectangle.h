#pragma once
#include "GraphicElement.h"

namespace graphic_sys_lib {
    class Rectangle
    : public GraphicElement {
    public:
        inline Rectangle() : GraphicElement() {
            mProperties.pushComponent<PosComponent>();
            mColor = RAYWHITE;
        }
        inline Rectangle(int posX, int posY) : GraphicElement() {
            mProperties.getComponent<PosComponent>(0).setPos(posX, posY);
            mProperties.pushComponent<PosComponent>();
            mColor = RAYWHITE;
        }
        inline Rectangle(int posX, int posY,int width, int height) : GraphicElement() {
            mProperties.getComponent<PosComponent>(0).setPos(posX, posY);
            mProperties.pushComponent<PosComponent>().setPos(width, height);
            mColor = RAYWHITE;
        }
        inline Rectangle(int posX, int posY, int width, int height, Color color) : GraphicElement() {
            mProperties.getComponent<PosComponent>(0).setPos(posX, posY);
            mProperties.pushComponent<PosComponent>().setPos(width, height);
            mColor = color;
        }

        inline void render() const override {
            auto comp = mProperties.getComponent<PosComponent>(0);
            auto comp1 = mProperties.getComponent<PosComponent>(1);
            DrawRectangle(comp.getPosX<int>(), comp.getPosY<int>(), comp1.getPosX<int>(), comp1.getPosY<int>(), mColor);
        }

        void parseFile(const std::string& pathToCMPT) override;
        _NODISCARD bool isParsableFile(const std::string& pathToCMPT) override;
        void parseFloatVar(const hardware_envi_lib::float_var floatVar) override;
        _NODISCARD bool isParsableFloatVar(const hardware_envi_lib::float_var floatVar) override;

        inline void setColor(Color color) noexcept {
            mColor = color;
        }

        inline _NODISCARD Color getColor() const noexcept {
            return mColor;
        }
    private:
        Color mColor;
    };
}
