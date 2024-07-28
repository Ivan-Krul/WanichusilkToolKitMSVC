#pragma once
#include "GraphicElement.h"

namespace graphic_sys_lib {
    class TextBox
        : public GraphicElement {
    public:
        inline TextBox() : GraphicElement() {
            mColor = RAYWHITE;
        }
        inline TextBox(const std::string& content) : GraphicElement() {
            mColor = RAYWHITE;
            mContent = content;
        }

        inline TextBox(const std::string& content, int posX, int posY) : GraphicElement() {
            mProperties.getComponent<PosComponent>().setPos(posX, posY);
            mContent = content;

            mColor = RAYWHITE;
        }
        inline TextBox(const std::string content,int posX, int posY, float fontSize) : GraphicElement() {
            mProperties.getComponent<PosComponent>().setPos(posX, posY);
            mColor = RAYWHITE;
            setFontSize(fontSize);
        }
        inline TextBox(const std::string content, int posX, int posY, Color color) : GraphicElement() {
            mProperties.getComponent<PosComponent>().setPos(posX, posY);
            mColor = color;
            mContent = content;
        }
        inline TextBox(const std::string& content, int posX, int posY, float fontSize, Color color) : GraphicElement() {
            mProperties.getComponent<PosComponent>().setPos(posX, posY);
            mColor = color;
            mContent = content;
            setFontSize(fontSize);
        }

        inline void render() const override {
            auto comp = mProperties.getComponent<PosComponent>();
            DrawText(mContent.c_str(), comp.getPosX<int>(), comp.getPosY<int>(), static_cast<int>(mFontSize), mColor);
        }

        void parseFile(const std::string& pathToCMPT) override;
        _NODISCARD bool isParsableFile(const std::string& pathToCMPT) override;
        void parseFloatVar(const hardware_envi_lib::float_var floatVar) override;
        _NODISCARD bool isParsableFloatVar(const hardware_envi_lib::float_var floatVar) override;

        inline _NODISCARD std::string& getContent() noexcept {
            return mContent;
        }
        inline _NODISCARD std::string getContent() const noexcept {
            return mContent;
        }

        inline _NODISCARD float getFontSize() const noexcept {
            return mFontSize;
        }
        inline void setFontSize(float size) noexcept {
            if (size == INFINITY) goto not_accepted;
            if (size < 0) goto not_accepted;
            mFontSize = size;
            return;

        not_accepted:
            std::clog << "log: no number in font size wasn't accepted\n";

        }

        inline _NODISCARD Color getColor() const noexcept {
            return mColor;
        }
        inline _NODISCARD Color& getColor() noexcept {
            return mColor;
        }
    private:
        Color mColor;
        std::string mContent;
        float mFontSize = 0.f;
    };
}
