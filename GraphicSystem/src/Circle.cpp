#include "../include/Circle.h"

namespace graphic_sys_lib {
    void Circle::parseFile(const std::string& pathToCMPT) {
        auto& compacter = parseCmptFile(pathToCMPT);
        auto& pos = mProperties.getComponent<PosComponent>();
        // PosComponent
        pos.setPosX(compacter["pos"][0].convertTo<float>());
        pos.setPosY(compacter["pos"][1].convertTo<float>());
        pos.setDirX(compacter["pos"][2].convertTo<float>());
        pos.setDirY(compacter["pos"][3].convertTo<float>());
        // Radius
        mRadius = compacter["radius"].convertTo<float>();
        // Color
        mColor = {
            compacter["color"][0].convertTo<unsigned char>(),
            compacter["color"][1].convertTo<unsigned char>(),
            compacter["color"][2].convertTo<unsigned char>(),
            compacter["color"][3].convertTo<unsigned char>()
        };
    }

    bool Circle::isParsableFile(const std::string& pathToCMPT) {
        auto& cmpt = parseCmptFile(pathToCMPT);
        if (cmpt.size() != 3) return false;
        if (cmpt.nameOfVariable(0) != "pos") return false;
        if (cmpt["pos"].data_type != cDTArr) return false;
        if (cmpt["pos"].size() != 4) return false;
        if (cmpt["pos"][0].data_type != cDTFlot) return false;
        if (cmpt["pos"][1].data_type != cDTFlot) return false;
        if (cmpt["pos"][2].data_type != cDTFlot) return false;
        if (cmpt["pos"][3].data_type != cDTFlot) return false;
        if (cmpt.nameOfVariable(1) != "radius") return false;
        if (cmpt["radius"].data_type != cDTFlot) return false;
        if (cmpt.nameOfVariable(2) != "color") return false;
        if (cmpt["color"].size() != 4) return false;
        if (cmpt["color"][0].data_type != cDTChar) return false;
        if (cmpt["color"][1].data_type != cDTChar) return false;
        if (cmpt["color"][2].data_type != cDTChar) return false;
        if (cmpt["color"][3].data_type != cDTChar) return false;
        return true;
    }

    void Circle::parseFloatVar(const hardware_envi_lib::float_var floatVar) {
        auto& pos = mProperties.getComponent<PosComponent>();
        pos.setPosX(floatVar.at(0).at(0).convertTo<float>());
        pos.setPosY(floatVar.at(0).at(1).convertTo<float>());
        pos.setDirX(floatVar.at(0).at(2).convertTo<float>());
        pos.setDirY(floatVar.at(0).at(3).convertTo<float>());

        mRadius = floatVar.at(1).convertTo<float>();

        mColor = {
            floatVar.at(2).at(0).convertTo<unsigned char>(),
            floatVar.at(2).at(1).convertTo<unsigned char>(),
            floatVar.at(2).at(2).convertTo<unsigned char>(),
            floatVar.at(2).at(3).convertTo<unsigned char>()
        };
    }

    bool Circle::isParsableFloatVar(const hardware_envi_lib::float_var floatVar) {
        if (floatVar.size() != 3) return false;
        if (floatVar.at(0).data_type != cDTArr) return false;
        if (floatVar.at(0).size() != 4) return false;
        if (floatVar.at(0).at(0).data_type != cDTFlot) return false;
        if (floatVar.at(0).at(1).data_type != cDTFlot) return false;
        if (floatVar.at(0).at(2).data_type != cDTFlot) return false;
        if (floatVar.at(0).at(3).data_type != cDTFlot) return false;
        if (floatVar.at(1).data_type != cDTFlot) return false;
        if (floatVar.at(2).size() != 4) return false;
        if (floatVar.at(2).at(0).data_type != cDTChar) return false;
        if (floatVar.at(2).at(1).data_type != cDTChar) return false;
        if (floatVar.at(2).at(2).data_type != cDTChar) return false;
        if (floatVar.at(2).at(3).data_type != cDTChar) return false;
        return true;
    }
}
