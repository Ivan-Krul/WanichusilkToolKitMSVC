#include "../include/Rectangle.h"

namespace graphic_sys_lib {
    void Rectangle::parseFile(const std::string& pathToCMPT) {
        auto& cmpt = parseCmptFile(pathToCMPT);

        auto& pos = mProperties.getComponent<PosComponent>();
        pos.setPosX(cmpt["pos"][0].convertTo<float>());
        pos.setPosY(cmpt["pos"][1].convertTo<float>());
        pos.setDirX(cmpt["pos"][2].convertTo<float>());
        pos.setDirY(cmpt["pos"][3].convertTo<float>());

        auto& size = mProperties.getComponent<PosComponent>();
        size.setPosX(cmpt["size"][0].convertTo<float>());
        size.setPosY(cmpt["size"][1].convertTo<float>());
        size.setDirX(cmpt["size"][2].convertTo<float>());
        size.setDirY(cmpt["size"][3].convertTo<float>());

        mColor = {
            cmpt["color"][0].convertTo<unsigned char>(),
            cmpt["color"][1].convertTo<unsigned char>(),
            cmpt["color"][2].convertTo<unsigned char>(),
            cmpt["color"][3].convertTo<unsigned char>()
        };
    }
    bool Rectangle::isParsableFile(const std::string& pathToCMPT) {
        auto& cmpt = parseCmptFile(pathToCMPT);
        if (cmpt.size() != 3) return false;
        if (cmpt.nameOfVariable(0) != "pos") return false;
        if (cmpt["pos"].data_type != cDTArr) return false;
        if (cmpt["pos"].size() != 4) return false;
        if (cmpt["pos"][0].data_type != cDTFlot) return false;
        if (cmpt["pos"][1].data_type != cDTFlot) return false;
        if (cmpt["pos"][2].data_type != cDTFlot) return false;
        if (cmpt["pos"][3].data_type != cDTFlot) return false;
        if (cmpt.nameOfVariable(1) != "size") return false;
        if (cmpt["size"].size() != 4) return false;
        if (cmpt["size"][0].data_type != cDTFlot) return false;
        if (cmpt["size"][1].data_type != cDTFlot) return false;
        if (cmpt["size"][2].data_type != cDTFlot) return false;
        if (cmpt["size"][3].data_type != cDTFlot) return false;
        if (cmpt.nameOfVariable(2) != "color") return false;
        if (cmpt["color"].size() != 4) return false;
        if (cmpt["color"][0].data_type != cDTChar) return false;
        if (cmpt["color"][1].data_type != cDTChar) return false;
        if (cmpt["color"][2].data_type != cDTChar) return false;
        if (cmpt["color"][3].data_type != cDTChar) return false;
        return true;
    }

    void Rectangle::parseFloatVar(const hardware_envi_lib::float_var floatVar) {
        auto& pos = mProperties.getComponent<PosComponent>();
        pos.setPosX(floatVar.at(0).at(0).convertTo<float>());
        pos.setPosY(floatVar.at(0).at(1).convertTo<float>());
        pos.setDirX(floatVar.at(0).at(2).convertTo<float>());
        pos.setDirY(floatVar.at(0).at(3).convertTo<float>());

        auto& size = mProperties.getComponent<PosComponent>();
        size.setPosX(floatVar.at(1).at(0).convertTo<float>());
        size.setPosY(floatVar.at(1).at(1).convertTo<float>());
        size.setDirX(floatVar.at(1).at(2).convertTo<float>());
        size.setDirY(floatVar.at(1).at(3).convertTo<float>());

        mColor = {
            floatVar.at(2).at(0).convertTo<unsigned char>(),
            floatVar.at(2).at(1).convertTo<unsigned char>(),
            floatVar.at(2).at(2).convertTo<unsigned char>(),
            floatVar.at(2).at(3).convertTo<unsigned char>()
        };
    }

    bool Rectangle::isParsableFloatVar(const hardware_envi_lib::float_var floatVar) {
        if (floatVar.size() != 3) return false;
        if (floatVar.at(0).data_type != cDTArr) return false;
        if (floatVar.at(0).size() != 4) return false;
        if (floatVar.at(0).at(0).data_type != cDTFlot) return false;
        if (floatVar.at(0).at(1).data_type != cDTFlot) return false;
        if (floatVar.at(0).at(2).data_type != cDTFlot) return false;
        if (floatVar.at(0).at(3).data_type != cDTFlot) return false;
        if (floatVar.at(1).size() != 4) return false;
        if (floatVar.at(1).at(0).data_type != cDTFlot) return false;
        if (floatVar.at(1).at(1).data_type != cDTFlot) return false;
        if (floatVar.at(1).at(2).data_type != cDTFlot) return false;
        if (floatVar.at(1).at(3).data_type != cDTFlot) return false;
        if (floatVar.at(2).size() != 4) return false;
        if (floatVar.at(2).at(0).data_type != cDTChar) return false;
        if (floatVar.at(2).at(1).data_type != cDTChar) return false;
        if (floatVar.at(2).at(2).data_type != cDTChar) return false;
        if (floatVar.at(2).at(3).data_type != cDTChar) return false;
        return true;
    }
}
