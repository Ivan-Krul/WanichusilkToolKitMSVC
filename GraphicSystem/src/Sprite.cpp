#include "../include/Sprite.h"

namespace graphic_sys_lib {
    void Sprite::parseFile(const std::string& pathToCMPT) {
        auto& compacter = parseCmptFile(pathToCMPT);
        auto& pos = mProperties.getComponent<PosComponent>();
        // PosComponent
        pos.setPosX(compacter["pos"][0].convertTo<float>());
        pos.setPosY(compacter["pos"][1].convertTo<float>());
        pos.setDirX(compacter["pos"][2].convertTo<float>());
        pos.setDirY(compacter["pos"][3].convertTo<float>());
        // Texture
        mTexture = LoadTexture(compacter["texture_path"]().c_str());
        // Tint
        mTint = {
            compacter["tint"][0].convertTo<unsigned char>(),
            compacter["tint"][1].convertTo<unsigned char>(),
            compacter["tint"][2].convertTo<unsigned char>(),
            compacter["tint"][3].convertTo<unsigned char>()
        };
    }

    bool Sprite::isParsableFile(const std::string& pathToCMPT) {
        auto& compacter = parseCmptFile(pathToCMPT);
        if (compacter.size() != 3) return false;
        if (compacter.nameOfVariable(0) != "pos") return false;
        if (compacter["pos"].data_type != cDTArr) return false;
        if (compacter["pos"].size() != 4) return false;
        if (compacter["pos"][0].data_type != cDTFlot) return false;
        if (compacter["pos"][1].data_type != cDTFlot) return false;
        if (compacter["pos"][2].data_type != cDTFlot) return false;
        if (compacter["pos"][3].data_type != cDTFlot) return false;
        if (compacter.nameOfVariable(1) != "texture_path") return false;
        if (compacter["texture_path"].data_type != cDTStr) return false;
        if (compacter.nameOfVariable(2) != "tint") return false;
        if (compacter["tint"].size() != 4) return false;
        if (compacter["tint"][0].data_type != cDTChar) return false;
        if (compacter["tint"][1].data_type != cDTChar) return false;
        if (compacter["tint"][2].data_type != cDTChar) return false;
        if (compacter["tint"][3].data_type != cDTChar) return false;
        return true;
    }

    void Sprite::parseFloatVar(const hardware_envi_lib::float_var floatVar) {
        auto& pos = mProperties.getComponent<PosComponent>();
        // PosComponent
        pos.setPosX(floatVar.at(0).at(0).convertTo<float>());
        pos.setPosY(floatVar.at(0).at(1).convertTo<float>());
        pos.setDirX(floatVar.at(0).at(2).convertTo<float>());
        pos.setDirY(floatVar.at(0).at(3).convertTo<float>());
        // Texture
        mTexture = LoadTexture(floatVar.at(1)().c_str());
        // Tint
        mTint = {
            floatVar.at(2).at(0).convertTo<unsigned char>(),
            floatVar.at(2).at(1).convertTo<unsigned char>(),
            floatVar.at(2).at(2).convertTo<unsigned char>(),
            floatVar.at(2).at(3).convertTo<unsigned char>()
        };
    }

    bool Sprite::isParsableFloatVar(const hardware_envi_lib::float_var floatVar) {
        if (floatVar.size() != 3) return false;
        if (floatVar.at(0).data_type != cDTArr) return false;
        if (floatVar.at(0).size() != 4) return false;
        if (floatVar.at(0).at(0).data_type != cDTFlot) return false;
        if (floatVar.at(0).at(1).data_type != cDTFlot) return false;
        if (floatVar.at(0).at(2).data_type != cDTFlot) return false;
        if (floatVar.at(0).at(3).data_type != cDTFlot) return false;
        if (floatVar.at(1).data_type != cDTStr) return false;
        if (floatVar.at(2).size() != 4) return false;
        if (floatVar.at(2).at(0).data_type != cDTChar) return false;
        if (floatVar.at(2).at(1).data_type != cDTChar) return false;
        if (floatVar.at(2).at(2).data_type != cDTChar) return false;
        if (floatVar.at(2).at(3).data_type != cDTChar) return false;
        return true;
    }
}
