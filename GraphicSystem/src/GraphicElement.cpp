#include "../include/GraphicElement.h"

#include <iostream>
#include <exception>

namespace graphic_sys_lib {
    GraphicElement::GraphicElement() {
        mConfig = parseCmptFile(CONFIG_PATH);

        mProperties.pushComponent<PosComponent>();
    }

    void GraphicElement::render() const {
        const auto posComp = mProperties.getComponent<PosComponent>(0);
        DrawRectangle(posComp.getPosX<int>(), posComp.getPosY<int>(), 1, 1, RED);
    }

    void GraphicElement::parseFile(const std::string& pathToCMPT) {
        auto& compacter = parseCmptFile(pathToCMPT);
        auto& pos = mProperties.getComponent<PosComponent>(0);
        // PosComponent
        pos.setPosX(compacter["pos"][0].convertTo<float>());
        pos.setPosY(compacter["pos"][1].convertTo<float>());
        pos.setDirX(compacter["pos"][2].convertTo<float>());
        pos.setDirY(compacter["pos"][3].convertTo<float>());
    }
    bool GraphicElement::isParsableFile(const std::string& pathToCMPT) {
        auto& compacter = parseCmptFile(pathToCMPT);
        if (compacter.size() != 1) return false;
        if (compacter.nameOfVariable(0) != "pos") return false;
        if (compacter["pos"].data_type != cDTArr) return false;
        if (compacter["pos"].size() != 4) return false;
        if (compacter["pos"][0].data_type != cDTFlot) return false;
        if (compacter["pos"][1].data_type != cDTFlot) return false;
        if (compacter["pos"][2].data_type != cDTFlot) return false;
        if (compacter["pos"][3].data_type != cDTFlot) return false;
        return true;
    }

    void GraphicElement::parseFloatVar(const hardware_envi_lib::float_var floatVar) {
        auto& pos = mProperties.getComponent<PosComponent>(0);
        // PosComponent
        pos.setPosX(floatVar.at(0).at(0).convertTo<float>());
        pos.setPosY(floatVar.at(0).at(1).convertTo<float>());
        pos.setDirX(floatVar.at(0).at(2).convertTo<float>());
        pos.setDirY(floatVar.at(0).at(3).convertTo<float>());
    }

    bool GraphicElement::isParsableFloatVar(const hardware_envi_lib::float_var floatVar) {
        if (floatVar.size() != 1) return false;
        if (floatVar.at(0).data_type != cDTArr) return false;
        if (floatVar.at(0).size() != 4) return false;
        if (floatVar.at(0).at(0).data_type != cDTFlot) return false;
        if (floatVar.at(0).at(1).data_type != cDTFlot) return false;
        if (floatVar.at(0).at(2).data_type != cDTFlot) return false;
        if (floatVar.at(0).at(3).data_type != cDTFlot) return false;
        return true;
    }
}
