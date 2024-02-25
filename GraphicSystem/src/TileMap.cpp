#include "../include/TileMap.h"

namespace graphic_sys_lib {
    void TileMap::render() const {
        const auto& comp = mProperties.getComponent<PosComponent>(0);
        const auto tileSizeX = static_cast<int>(mCommonTileSizeX);
        const auto tileSizeY = static_cast<int>(mCommonTileSizeY);

        for (int y = 0; y < mMap.size(); y++) {
            for (int x = 0; x < mMap[y].size(); x++) {
                Texture& tex = *mAssembledMap[x + y * mMap[y].size()];
                DrawTexture(tex, comp.getPosX<int>() + tileSizeX * x, comp.getPosY<int>() + tileSizeY * y, RAYWHITE);
            }
        }
    }

    void TileMap::assemble() noexcept {
        mAssembledMap.reserve(mMap.size() * mMap.begin()->size());
        for (auto& layer : mMap) {
            for (auto& tile : layer) {
                mAssembledMap.push_back(&mTileSet[tile]);
            }
        }
    }

    bool TileMap::isExistSet(int key) const noexcept {
        try {
            return mTileSet.find(key) != mTileSet.end();
        } catch (...) {
            return false;
        }
    }

    Texture2D& TileMap::getTexture(int key) {
        if (!isExistSet(key)) {
            std::cerr << "out of range: key in TileSet like " << key << " don't exist, so it refuses\n";
            throw std::out_of_range("Key in TileSet don't exist");
        }
        return mTileSet[key];
    }

    Texture2D TileMap::getTexture(int key) const {
        if (!isExistSet(key)) {
            std::cerr << "out of range: key in TileSet like " << key << " don't exist, so it refuses\n";
            throw std::out_of_range("Key in TileSet don't exist");
        }
        return mTileSet.at(key);
    }

    void TileMap::parseFile(const std::string& pathToCMPT) {
        auto& compacter = parseCmptFile(pathToCMPT);
        auto& pos = mProperties.getComponent<PosComponent>(0);
        // PosComponent
        pos.setPosX(compacter["pos"][0].convertTo<float>());
        pos.setPosY(compacter["pos"][1].convertTo<float>());
        pos.setDirX(compacter["pos"][2].convertTo<float>());
        pos.setDirY(compacter["pos"][3].convertTo<float>());
        // CommonTileSize
        mCommonTileSizeX = compacter["common_tile_size"][0].convertTo<unsigned>();
        mCommonTileSizeY = compacter["common_tile_size"][1].convertTo<unsigned>();
        // TileSet
        auto& tile_set = compacter["tile_set"];
        for (size_t i = 0; i < tile_set[0].size(); i++) {
            mTileSet[tile_set[0][i].convertTo<int>()] = LoadTexture(tile_set[1][i]().c_str());
        }
        // Map
        auto& map = compacter["map"];
        mMap.reserve(map.size());
        for (size_t y = 0; y < map.size(); y++) {
            mMap.push_back(std::vector<int>());
            mMap.back().reserve(map[y].size());
            for (size_t x = 0; x < map[y].size(); x++) {
                mMap[y].push_back(map[y][x].convertTo<int>());
            }
        }
    }

    bool TileMap::isParsableFile(const std::string& pathToCMPT) {
        auto& compacter = parseCmptFile(pathToCMPT);
        if (compacter.size() != 4) return false;
        if (compacter.nameOfVariable(0) != "pos") return false;
        if (compacter["pos"].data_type != cDTArr) return false;
        if (compacter["pos"].size() != 4) return false;
        if (compacter["pos"][0].data_type != cDTFlot) return false;
        if (compacter["pos"][1].data_type != cDTFlot) return false;
        if (compacter["pos"][2].data_type != cDTFlot) return false;
        if (compacter["pos"][3].data_type != cDTFlot) return false;
        if (compacter.nameOfVariable(1) != "common_tile_size") return false;
        if (compacter["common_tile_size"].data_type != cDTArr) return false;
        if (compacter["common_tile_size"].size() != 4) return false;
        if (compacter["common_tile_size"][0].data_type != cDTUInt) return false;
        if (compacter["common_tile_size"][1].data_type != cDTUInt) return false;
        if (compacter.nameOfVariable(2) != "tile_set") return false;
        if (compacter["tile_set"].data_type != cDTArr) return false;
        if (compacter["tile_set"][0].data_type != cDTArr) return false;
        if (compacter["tile_set"][1].data_type != cDTArr) return false;
        for (auto& el : compacter["tile_set"][0].value.arr) {
            if (el.data_type != cDTInt) return false;
        }
        for (auto& el : compacter["tile_set"][1].value.arr) {
            if (el.data_type != cDTStr) return false;
        }
        if (compacter.nameOfVariable(2) != "map") return false;
        if (compacter["map"].data_type != cDTArr) return false;
        for (auto& el : compacter["map"].value.arr) {
            if (el.data_type != cDTArr) return false;
            for (auto& el1 : el.value.arr) {
                if (el1.data_type != cDTInt) return false;
            }
        }
        return true;
    }

    void TileMap::parseFloatVar(const hardware_envi_lib::float_var floatVar) {
        auto& pos = mProperties.getComponent<PosComponent>(0);
        // PosComponent
        pos.setPosX(floatVar.at(0).at(0).convertTo<float>());
        pos.setPosY(floatVar.at(0).at(1).convertTo<float>());
        pos.setDirX(floatVar.at(0).at(2).convertTo<float>());
        pos.setDirY(floatVar.at(0).at(3).convertTo<float>());
        // CommonTileSize
        mCommonTileSizeX = floatVar.at(1).at(0).convertTo<unsigned>();
        mCommonTileSizeY = floatVar.at(1).at(1).convertTo<unsigned>();
        // TileSet
        auto tile_set = floatVar.at(2);
        for (size_t i = 0; i < tile_set.at(0).size(); i++) {
            mTileSet[tile_set[0][i].convertTo<int>()] = LoadTexture(tile_set[1][i]().c_str());
        }
        // Map
        auto map = floatVar.at(3);
        mMap.reserve(map.size());
        for (size_t y = 0; y < map.size(); y++) {
            mMap.push_back(std::vector<int>(map[y].size()));
            for (size_t x = 0; x < map[y].size(); x++) {
                mMap[y].push_back(map[y][x].convertTo<int>());
            }
        }
    }

    bool TileMap::isParsableFloatVar(const hardware_envi_lib::float_var floatVar) {
        if (floatVar.size() != 4) return false;
        if (floatVar.at(0).data_type != cDTArr) return false;
        if (floatVar.at(0).size() != 4) return false;
        if (floatVar.at(0).at(0).data_type != cDTFlot) return false;
        if (floatVar.at(0).at(1).data_type != cDTFlot) return false;
        if (floatVar.at(0).at(2).data_type != cDTFlot) return false;
        if (floatVar.at(0).at(3).data_type != cDTFlot) return false;
        if (floatVar.at(1).data_type != cDTArr) return false;
        if (floatVar.at(1).size() != 4) return false;
        if (floatVar.at(1).at(0).data_type != cDTUInt) return false;
        if (floatVar.at(1).at(1).data_type != cDTUInt) return false;
        if (floatVar.at(2).data_type != cDTArr) return false;
        if (floatVar.at(2).at(0).data_type != cDTArr) return false;
        if (floatVar.at(2).at(1).data_type != cDTArr) return false;
        for (auto& el : floatVar.at(2).at(0).value.arr) {
            if (el.data_type != cDTInt) return false;
        }
        for (auto& el : floatVar.at(2).at(1).value.arr) {
            if (el.data_type != cDTStr) return false;
        }
        if (floatVar.at(3).data_type != cDTArr) return false;
        for (auto& el : floatVar.at(3).value.arr) {
            if (el.data_type != cDTArr) return false;
            for (auto& el1 : el.value.arr) {
                if (el1.data_type != cDTInt) return false;
            }
        }
        return true;
    }

    TileMap::~TileMap() {
        for (auto& set : mTileSet) {
            UnloadTexture(set.second);
        }
    }
}
