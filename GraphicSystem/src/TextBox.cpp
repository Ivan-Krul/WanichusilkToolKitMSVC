#include "../include/TextBox.h"

namespace graphic_sys_lib {
    void TextBox::parseFile(const std::string& pathToCMPT) {
        auto& cmpt = parseCmptFile(pathToCMPT);

        auto& pos = mProperties.getComponent<PosComponent>(0);
        pos.setPosX(cmpt["pos"][0].convertTo<float>());
        pos.setPosY(cmpt["pos"][1].convertTo<float>());
        pos.setDirX(cmpt["pos"][2].convertTo<float>());
        pos.setDirY(cmpt["pos"][3].convertTo<float>());

        mContent = cmpt["content"]();
        mFontSize = cmpt["font_size"].convertTo<float>();

        mColor = {
            cmpt["color"][0].convertTo<unsigned char>(),
            cmpt["color"][1].convertTo<unsigned char>(),
            cmpt["color"][2].convertTo<unsigned char>(),
            cmpt["color"][3].convertTo<unsigned char>()
        };
    }

    bool TextBox::isParsableFile(const std::string& pathToCMPT) {
        auto& cmpt = parseCmptFile(pathToCMPT);
        if (cmpt.size() != 4) return false;
        if (cmpt.nameOfVariable(0) != "pos") return false;
        if (cmpt["pos"].data_type != cDTArr) return false;
        if (cmpt["pos"].size() != 4) return false;
        if (cmpt["pos"][0].data_type != cDTFlot) return false;
        if (cmpt["pos"][1].data_type != cDTFlot) return false;
        if (cmpt["pos"][2].data_type != cDTFlot) return false;
        if (cmpt["pos"][3].data_type != cDTFlot) return false;
        if (cmpt.nameOfVariable(1) != "content") return false;
        if (cmpt["content"].data_type != cDTStr) return false;
        if (cmpt.nameOfVariable(2) != "font_size") return false;
        if (cmpt["font_size"].data_type != cDTFlot) return false;
        if (cmpt.nameOfVariable(3) != "color") return false;
        if (cmpt["color"].size() != 4) return false;
        if (cmpt["color"][0].data_type != cDTChar) return false;
        if (cmpt["color"][1].data_type != cDTChar) return false;
        if (cmpt["color"][2].data_type != cDTChar) return false;
        if (cmpt["color"][3].data_type != cDTChar) return false;
        return true;
    }

    void TextBox::parseFloatVar(const hardware_envi_lib::float_var floatVar) {
        auto& pos = mProperties.getComponent<PosComponent>(0);
        pos.setPosX(floatVar.at(0).at(0).convertTo<float>());
        pos.setPosY(floatVar.at(0).at(1).convertTo<float>());
        pos.setDirX(floatVar.at(0).at(2).convertTo<float>());
        pos.setDirY(floatVar.at(0).at(3).convertTo<float>());

        mContent = floatVar.at(1)();
        mFontSize = floatVar.at(2).convertTo<float>();

        mColor = {
            floatVar.at(3).at(0).convertTo<unsigned char>(),
            floatVar.at(3).at(1).convertTo<unsigned char>(),
            floatVar.at(3).at(2).convertTo<unsigned char>(),
            floatVar.at(3).at(3).convertTo<unsigned char>()
        };
    }
    bool TextBox::isParsableFloatVar(const hardware_envi_lib::float_var floatVar) {
        if (floatVar.size() != 4) return false;
        if (floatVar.at(0).data_type != cDTArr) return false;
        if (floatVar.at(0).size() != 4) return false;
        if (floatVar.at(0).at(0).data_type != cDTFlot) return false;
        if (floatVar.at(0).at(1).data_type != cDTFlot) return false;
        if (floatVar.at(0).at(2).data_type != cDTFlot) return false;
        if (floatVar.at(0).at(3).data_type != cDTFlot) return false;
        if (floatVar.at(1).data_type != cDTStr) return false;
        if (floatVar.at(2).data_type != cDTFlot) return false;
        if (floatVar.at(3).size() != 4) return false;
        if (floatVar.at(3).at(0).data_type != cDTChar) return false;
        if (floatVar.at(3).at(1).data_type != cDTChar) return false;
        if (floatVar.at(3).at(2).data_type != cDTChar) return false;
        if (floatVar.at(3).at(3).data_type != cDTChar) return false;
        return true;
    }
}
